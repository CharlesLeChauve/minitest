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

// char	*get_env_value(char **env, char *var_id)
// {
// 	int		i;
// 	int		id_len;
// 	char	*value

// 	id_len = ft_strlen(var_id);
// 	i = -1;
// 	while (env[++i])
// 	{
// 		if (!ft_strncmp(env[i], var_id, id_len))
// 			break ;
// 	}
// 	if (env[i])
// 		return (env[i]);
// 	return (NULL);
// }

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

int	no_equal(char *var)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	while (var[i])
	{
		if (var[i] == '=')
			equal = 1;
		i++;
	}
	if (equal == 0)
		return (1);
	return (0);
}

int	print_env(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (no_equal(env[i]))
		{
			i++;
			continue ;
		}
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
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

void remove_from_tab(char ***arg, int index)
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

char *extract_var_id(char *var)
{
	char *var_id;
	int i = 0;
	int	index = -1;
	
	while (var[i])
	{
		if (var[i] == '=')
		{
			index = i;
			break ;
		}
		i++;
	}
	if (index > -1)
		return (ft_strndup(var, index + 1));
	return (var);
}

void	replace_existing_vars(char ***arg, char ***env)
{
	int		i;
	char	*var;
	char	*var_id;

	i = 0;
	var = NULL;
	while ((*arg)[i])
	{
		var_id = extract_var_id((*arg)[i]);
		var = get_env_var(*env, var_id);
		if (var)
		{
			replace_var(env, (*arg)[i], var);
			remove_from_tab(arg, i);
		}
		else
			i++;
		var = NULL;
	}
}

char	*export_format(char *str)
{
	char *formated;
	int	i;
	int	j;

	i = 0;
	j = 0;
	formated = (char *)malloc(ft_strlen(str) + 3);
	while (str[i] && str[i - 1] != '=')
		formated[j++] = str[i++];
	if (str[i] == '\0' && !(str[i - 1] == '='))
		return (formated);
	formated[j++] = '"';
	while (str[i])
	{
		formated[j] = str[i];
		i++;
		j++;
	}
	formated[j++] = '"';
	formated[j++] = '\0';
	return (formated);
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
    if (*arg == NULL)
	{
		while ((*env)[i])
			i++;
		env_cpy = (char **)malloc((i + 1) * sizeof(char *));
		i = -1;
		while ((*env)[++i])
			env_cpy[i] = export_format((*env)[i]);	//ft_strdup((*env)[i]);
		env_cpy[i] = NULL;
        print_export_env(env_cpy);
		
	}
    else
    {
		replace_existing_vars(&arg, env);
        add_vars_to_env(arg, env);
    }
	return (0);
}

int	unset(char ***env, char **args)
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
				remove_from_tab(env, env_idx);
				remove_from_tab(&args, i);
		}
		else
			i++;
    }
	return (0);
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
