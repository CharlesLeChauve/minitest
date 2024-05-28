#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"
#include "env.h"
#include "minishell.h"
#include <stdlib.h>

void	ft_sort_wordtab(char **tab)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (tab[i] != 0)
	{
		j = i + 1;
		while (tab[j] != 0)
		{
			if (ft_strcmp(tab[i], tab[j]) > 0)
			{
				temp = tab[i];
				tab[i] = tab[j];
				tab[j] = temp;
			}
			++j;
		}
		++i;
	}
}

void	ft_remove_from_strtab(char **tab, int index)
{
	int	size;

	size = 0;
	while (tab[size])
		size++;
	free(tab[index]);
	while (index < size - 1)
	{
		tab[index] = tab[index + 1];
		index++;
	}
	tab[size - 1] = NULL;
}

void    *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;
	size_t copy_size;

	new_ptr = NULL;
	new_ptr = malloc(new_size);
	if (!new_ptr)
		{
			perror("malloc failed\n");
			exit(EXIT_FAILURE);
		}
	if (!ptr)
		return (new_ptr);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}


char	*get_env_var(char **env, char *var_id)
{
	int		i;
	char	*env_var;
	int		id_len;

	id_len = ft_strlen(var_id);
	env_var = NULL;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var_id, id_len))
		{
			env_var = env[i];
			break ;
		}
	}
	if (env_var)
		return (env[i]);
	return (NULL);
}

int	get_env_index(char **env, char *var_id)
{
	int		i;
	char	*env_var;
	int		id_len;

	id_len = ft_strlen(var_id);
	env_var = NULL;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var_id, id_len))
		{
			env_var = env[i];
			break ;
		}
	}
	if (env_var)
		return (i);
	return (-1);
}

void	print_env(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
}

void	print_export_env(char **env)
{
	int	i;

	i = 0;
	ft_sort_wordtab(env);
	while (env[i])
	{
		if (strncmp(env[i], "_=", 2) == 0) 
		{
            i++;
            continue ;
		}
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
}

void	add_vars_to_env(char **arg, char ***env)
{
	int	i;
	int	env_size;
	int	total_size;
	int	arg_size;

	arg_size = 0;
	env_size = 0;
	i = 0;
	while ((*env)[env_size])
		env_size++;
	while (arg[arg_size])
		arg_size++;
	total_size = env_size + arg_size;
	*env = ft_realloc(*env, (env_size) * sizeof (char *), (total_size + 1) * sizeof (char *));
	while (i < arg_size)
	{
		(*env)[env_size + i] = ft_strdup(arg[i]);
		i++;
	}
	(*env)[env_size + i] = NULL;
}

void	replace_var(char ***env, char *new_var, char *old_var)
{
	int	i;

	i = 0;
	while (ft_strcmp((*env)[i], old_var))
		i++;
	free((*env)[i]);
	(*env)[i] = ft_strdup(new_var);
}

void remove_arg(char ***arg, int index)
{
	char **temp;

    if (arg == NULL || *arg == NULL || (*arg)[index] == NULL)
        return ;

    temp = *arg;

    int i = index;
    while (temp[i + 1])
    {
        temp[i] = temp[i + 1];
        i++;
    }
    temp[i] = NULL;

    *arg = temp; 
}

void	replace_existing_vars(char ***arg, char ***env)
{
	int		i;
	char	*var;

	i = 0;
	var = NULL;
	while ((*arg)[i])
	{
		var = get_env_var(*env, (*arg)[i]);
		if (var)
		{
			replace_var(env, (*arg)[i], var);
			remove_arg(arg, i);
		}
		else
			i++;
		var = NULL;
	}
}

//export OK
//Usage : Premier argument est un pointeur vers les tableau de chaines de caracteres env
// Deuxieme argument un tableau NULL termine de chaines de caracteres NULL terminees listant les variables a ajouter;

int    export(char **env[], char **arg)
{
    char    **new_vars;
    char    **env_cpy;
    int     i;

    i = 0;
    while ((*env)[i])
        i++;
    env_cpy = (char **)malloc((i + 1) * sizeof(char *));
    i = -1;
    while ((*env)[++i])
        env_cpy[i] = ft_strdup((*env)[i]);
    env_cpy[i] = NULL;
    if (*arg == NULL)
        print_export_env(env_cpy);
    else
    {
		// if (not_a_valid_identifier(arg))
		// 	return (1);
		replace_existing_vars(&arg, &env_cpy);
        add_vars_to_env(arg, &env_cpy);
		ft_free_tab(*env);
		*env = env_cpy;
    }
	return (0);
}

void	unset(char ***env, char **args)
{
	char	**new_env;
	int		i;
	int		env_idx;

	i = 0;
	env_idx = 0;
	while (args[i])
	{
		env_idx = get_env_index(*env, args[i]);
		if (env_idx != -1)
		{
				ft_remove_from_strtab(*env, env_idx);
				ft_remove_from_strtab(args, i);
		}
		else
			i++;
    }
}

// int main(int argc, char *argv[], char *envp[])
// {
//     char    **env;
//     char    *env_pwd;
//     char    *args[] = {"ARG=\"fuck\"", "FUCK=\"arg\"", NULL};
// 	char    *args2[] = {"ARG=\"arg\"", "FUCK=\"fuck\"", NULL};
//     int     i;

//     i = 0;
//     while (envp[i])
//         i++;
//     env = (char **)malloc(sizeof(char *) * (i + 1));
//     i = 0;
//     while (envp[i])
//     {
//         env[i] = ft_strdup(envp[i]);
//         i++;
//     }
//     env[i] = NULL;
// 	pwd();
//     if (change_directory("/home/", env))
//     {
//         printf("cd failed\n");
//     }
//     export(&env, args);
//     export(&env, NULL);
// 	export(&env, args2);
// 	export(&env, NULL);
// }
