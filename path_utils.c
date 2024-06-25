#include "minishell.h"

char	*get_env_path(char **env)
{
	int		i;
	char	*env_path;

	i = -1;
	env_path = NULL;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			env_path = ft_strdup(&env[i][5]);
	}
	return (env_path);
}

char	*get_cmd_path(char **env, char *cmd)
{
	char	*paths;
	char	**paths_array;
	char	*cmd_path;
	int		i;

	i = 0;
	paths = get_env_path(env);
	if (paths == NULL)
		return (NULL);
	paths_array = ft_split(paths, ':');
	free(paths);
	while (paths_array[i])
	{
		cmd_path = ft_strjoin(paths_array[i], "/");
		cmd_path = ft_strjoin_free(cmd_path, cmd, 0);
		if (access(cmd_path, X_OK) != -1)
		{
			ft_free_tab(paths_array);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_tab(paths_array);
	return (NULL);
}

int is_a_path(char *cmd)
{
	if (!access(cmd, F_OK))
		return (1);
	return (0);
}

char    *set_cmd_path(char *envp[], char *cmd)
{
	if ((*cmd == '.' || *cmd == '/') && is_a_path(cmd))
		return (cmd);
	return (get_cmd_path(envp, cmd));
}
