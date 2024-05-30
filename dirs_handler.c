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

void	act_env_pwd(char ***env, char *pwd)
{
	char **pwds;

	pwds[0] = ft_strjoin_free("PWD=", get_cwd(), 1);
	pwds[1] = ft_strjoin_free("OLDPWD=", ft_strdup(get_env_var(*env, "PWD") + 4), 1);
	pwds[2] = NULL;
	replace_existing_vars(&pwds, env);
}

int change_directory(char *path, char ***env)
{
	if (chdir(path))
	{
		perror("chdir() error\n");
		return (1);
	}
	else
	{
		act_env_pwd(env, get_cwd());
	}
	return (0);
}