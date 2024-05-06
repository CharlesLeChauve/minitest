#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"
#include "env.h"


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
		ft_realloc(path, buffer_size, buffer_size + buffer_size);
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
	while (*env)
	{
		ft_printf("declare -x %s\n", *env);
		env++;
	}
}

void	export(char **env, char *arg)
{
	//char	**new_vars;
	char **env_cpy;
	int	i;

	i = 0;
	while (env[i])
	{
		i++;
	}
	env_cpy = (char **)malloc((i + 1) * sizeof (char *));
	i = 0;
	while (env[i])
	{
		env_cpy[i] = ft_strdup(env[i]);
		i++;
	}
	ft_sort_wordtab(env_cpy);
	if (arg == NULL)
		print_export_env(env_cpy);
	// else
	// {

	// }
}

int main(int argc, char *argv[], char **env)
{
	char	*path;
	char	*env_pwd;

	(void)argc;
	(void)argv;
	path = get_cwd();
	env_pwd = get_env_var(env, "PWD");
	printf("cwd : %s\tenv : %s\n", path, env_pwd);
	if (change_directory("/home/", env))
	{
		printf("cd failed\n");
	}
	free(path);
	get_cwd();
	env_pwd = get_env_var(env, "PWD");
	printf("cwd : %s\tenv : %s\n", path, env_pwd);
	export(env, NULL);
}
