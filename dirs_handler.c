#include "minishell.h"
#include "env.h"

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

void	pwd(void)
{
	char	*wd;

	wd = get_cwd();
	printf("%s\n", wd);
	free(wd);
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
	pwd = get_cwd();
	tmp = ft_strjoin_free(new_pwd, pwd, 2);
	free(*env);
	*env = tmp;
}

int change_directory(char *path, char **env)
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