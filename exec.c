# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include "minishell.h"

char	*get_env_path(char **env)
{
	int		i;
	char	*env_path;

	i = -1;
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
	paths_array = ft_split(paths, ':');
	free(paths);
	while (paths_array[i])
	{
		cmd_path = ft_strjoin(paths_array[i], "/");
		cmd_path = ft_strjoin(cmd_path, cmd);
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

int	open_mode(char *path, t_open_mode mode)
{
	int	fd;

	if (mode == read_o)
	{
		fd = open(path, O_RDONLY);
		return (fd);
	}
	if (mode == truncate_o)
	{
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		return (fd);
	}
	if (mode == append_o)
	{
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		return (fd);
	}
	return (-1);
}

int	open_write(char *file, t_open_mode mode)
{
	int	fd;

	if (access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_printf("permission denied : %s\n", file);
		exit(EXIT_FAILURE);
	}
	if (mode == truncate_o)
		fd = open_mode(file, truncate_o);
	else if (mode == append_o)
		fd = open_mode(file, append_o);
	else
		fd = -1;
	return (fd);
}

void    make_redir(t_token_lst *redir)
{
    int fd;

    fd = -1;
    if (redir->type == redir_out)
    {
        fd = open_write(redir->text, truncate_o);
        if (fd == -1)
        {
            perror("open_write");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
    else if (redir->type == redir_app)
    {
        fd = open_write(redir->text, append_o);
        if (fd == -1)
        {
            perror("open_write");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
}

int exec_command_and_redirs(t_cmd_block *cmd_block, char *envp[])
{
    pid_t   pid;
    int     status;
    t_list  *redirs;

    create_exec_tab(cmd_block);
    if ((pid = fork()) == -1) 
    {
        perror("fork");
        return (-1);
    }
    if (pid == 0) 
    {
        if (cmd_block->redir_out)
        {
            redirs = cmd_block->redir_out;
            while (redirs->next)
            {
                make_redir((t_token_lst *)redirs->content);
                redirs = redirs->next;
            }
        }
        execve(get_cmd_path(envp, cmd_block->exec_tab[0]), cmd_block->exec_tab, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (waitpid(pid, &status, 0) == -1) 
        {
            perror("waitpid");
            return (-1);
        }
    }
    return (-1);
}

int	exec_ast(t_ast_node *ast, char *envp[])
{
    int ret_value;

    ret_value = 0;
    if (ast->type == pipe_op ) // && pas de redirection de sortie a gauche
    {
        int pipe_fds[2];
        pid_t pid1, pid2;
        if (pipe(pipe_fds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if ((pid1 = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid1 == 0)
        {
            close(pipe_fds[0]);
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[1]);
            exit(exec_ast(ast->left, envp));
        }
        if ((pid2 = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid2 == 0)
        {
            close(pipe_fds[1]);
            dup2(pipe_fds[0], STDIN_FILENO);
            close(pipe_fds[0]);
            exit(exec_ast(ast->right, envp));
        }
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return 0;
    }
    else if (ast->type == and_op && !exec_ast(ast->left, envp))
        return (exec_ast(ast->right, envp));
    else if (ast->type == or_op && exec_ast(ast->left, envp))
    {
        ret_value = exec_ast(ast->left, envp);
        if (ret_value)
            return (exec_ast(ast->right, envp));
        return (ret_value);
    }
    // else if (is_a_builtin(ast->cmd_block))
    //     return (exec_builtin(ast->cmd_block));
    else
        return (exec_command_and_redirs(ast->cmd_block, envp));
}
