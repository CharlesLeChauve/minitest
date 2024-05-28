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

void	act_env_pwd(char **env, char *pwd)
{
	char *old_pwd;
	char *new_pwd;

	new_pwd = ft_strdup("PWD=");
	old_pwd = get_env_var(env, "PWD");
	new_pwd = ft_strjoin_free(new_pwd, pwd, 2);
	free(old_pwd);
	old_pwd = new_pwd;
}

int change_directory(char *path, char **env)
{
	if (chdir(path))
	{
		perror("chdir() error\n");
		exit (EXIT_FAILURE);
	}
	else
	{
		act_env_pwd(env, get_cwd());
	}
	//exit (EXIT_SUCCESS);
}