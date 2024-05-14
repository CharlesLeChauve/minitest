#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"
#include "env.h"
#include <stdlib.h>

void    ft_putstr(char *str)
{
    while (*str)
    {
        write(1, str, 1);
        str++;
    }
}

void    print_str_tab(char **tab)
{
    while (*tab)
    {
        ft_putstr(*tab);
        write(1, "\n", 1);
        tab++;
    }
}


int		ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		++i;
	return (s1[i] - s2[i]);
}

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
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var_id, id_len))
			break ;
	}
	return (env[i]);
}

char	*get_cwd(void)
{
	size_t  buffer_size;
	char	*path;

	buffer_size = CWD_BUFF;
	path = (char *)malloc(CWD_BUFF);
	while (!getcwd(path, buffer_size))
	{
		path = ft_realloc(path, buffer_size, buffer_size + buffer_size);
		buffer_size += buffer_size;
	}
	return (path);
}

void	act_env_pwd(char **env)
{
	char *pwd;
	char *new_pwd;
	char *tmp;

	new_pwd = ft_strdup("PWD=");
	pwd = get_env_var(env, "PWD");
	while (*env != pwd)
		env++;
	tmp = ft_strjoin_free(new_pwd, get_cwd(), 0);
	*env = tmp;
}

int    change_directory(char *path, char **env)
{
	if (chdir(path))
	{
		perror("chdir() error\n");
		return (1);
	}
	else
		act_env_pwd(env);
	return (0);
}

void	print_export_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
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
	{
		printf("arg[%d] = %s\n", i, arg[i]);
		arg_size++;
	}
	total_size = env_size + arg_size;
	*env = realloc(*env, (total_size + 1) * sizeof (char *));
	while (i < arg_size)
	{
		(*env)[env_size + i] = ft_strdup(arg[i]);
		i++;
	}
	(*env)[env_size + i] = NULL;
}

//export OK
//Usage : Premier argument est un pointeur vers les tableau de chaines de caracteres env
// Deuxieme argument un tableau NULL termine de chaines de caracteres NULL terminees listant les variables a ajouter;

void    export(char ***env, char **arg)
{
    char    **new_vars;
    char    **env_cpy;
    int     i;

    i = 0;
    while ((*env)[i])
        i++;
    env_cpy = (char **)malloc((i + 1) * sizeof(char *));
    i = 0;
    while ((*env)[i])
    {
        env_cpy[i] = ft_strdup((*env)[i]);
        i++;
    }
    env_cpy[i] = NULL;

    if (arg == NULL)
    {
        ft_sort_wordtab(env_cpy);  // Supposé trier les variables pour l'affichage
        print_export_env(env_cpy); // Supposé imprimer les variables d'environnement
    }
    else
    {
        add_vars_to_env(arg, &env_cpy);  // Modifié pour potentiellement ajuster la taille de env_cpy
    }
    free(*env);  // Libérer l'ancien environnement
    *env = env_cpy;  // Mettre à jour le pointeur d'environnement original
}

int main(int argc, char *argv[], char *envp[])
{
    char    *path;
    char    **env;
    char    *env_pwd;
    char    *args[] = {"ARG=\"fuck\"", "FUCK=\"arg\"", NULL};
    int     i;

    i = 0;
    while (envp[i])
        i++;
    env = (char **)malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (envp[i])
    {
        env[i] = ft_strdup(envp[i]);
        i++;
    }
    env[i] = NULL;

    path = get_cwd();  // Supposé récupérer le chemin actuel
    env_pwd = get_env_var(env, "PWD");
    printf("cwd : %s\tenv : %s\n", path, env_pwd);
    if (change_directory("/home/", env))  // Modifié pour passer l'adresse de env
    {
        printf("cd failed\n");
    }
    path = get_cwd();  // Rafraîchir path après le changement de répertoire
    env_pwd = get_env_var(env, "PWD");
    printf("cwd : %s\tenv : %s\n", path, env_pwd);
    export(&env, NULL);
    export(&env, args);
    export(&env, NULL);

    free(path);
    }
