# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include "libft/libft.h"
# include "minishell.h"

int is_a_builtin(char *command)
{
	if (!ft_strcmp(command, "cd") || !ft_strcmp(command, "echo")
			|| !ft_strcmp(command, "env") || !ft_strcmp(command, "exit")
			|| !ft_strcmp(command, "export") || !ft_strcmp(command, "pwd")
			|| !ft_strcmp(command, "unset"))
		return(1);
	return (0);
}

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

void	read_heredoc(char *limiter)
{
	char	*nl;

	while (1)
	{
		nl = get_next_line(0);
		if (nl == NULL)
		{
			ft_putstr_fd(
				"Error : End Of File before finding here_doc LIMITER", 2);
			exit(EXIT_FAILURE);
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter))
			&& nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			exit(0);
		}
		ft_putstr_fd(nl, 1);
		free(nl);
	}
}

void	heredoc_handle(char *limiter)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		exit(-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		read_heredoc(limiter);
	}
	else if (pid > 0)
	{
		waitpid(pid, &(int){0}, 0);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
	}
}

int	check_acces(char *file, t_open_mode mode)
{
	if ((mode == truncate_o || mode == append_o) && access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_printf("permission denied : %s\n", file);
		return (0);
	}
	else if (mode == read_o && access(file, F_OK) != 0)
	{
		ft_printf("no such file or directory : %s\n", file);
		return (0);
	}
	if (mode == read_o && access(file, R_OK) != 0)
	{
		ft_printf("permission denied : %s\n", file);
		return (0);
	}
	return (1);
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

int    make_redir_in(t_token_lst *redir)
{
	int fd;

	fd = -1;
	if (redir->type == redir_in)
	{
		if (check_acces(redir->text, read_o))
		{
			fd = open_mode(redir->text, read_o);
			dup2(fd, STDIN_FILENO);
		}
		else
			return (1);
	}
	else if (redir->type == heredoc)
	{
		heredoc_handle(redir->text);
	}
	return (0);
}

int    make_redir_out(t_token_lst *redir)
{
	int fd;

	fd = -1;
	if (redir->type == redir_out)
	{
		fd = open_write(redir->text, truncate_o);
		dup2(fd, STDOUT_FILENO);
	}
	else if (redir->type == redir_app)
	{
		fd = open_write(redir->text, append_o);
		if (fd == -1)
		{
			perror("open_write");
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
}

void    handle_redirs(t_cmd_block *cmd_block)
{
	t_list  *redirs;

	if (cmd_block->redirs)
	{
		redirs = cmd_block->redirs;
		while (redirs)
		{
			//si l'une des redirs ne peut pas se produire
			if (((t_token_lst *)redirs->content)->type == redir_app || ((t_token_lst *)redirs->content)->type == redir_out)
				if (make_redir_out((t_token_lst *)redirs->content))
					exit(EXIT_FAILURE) ;
			if (((t_token_lst *)redirs->content)->type == redir_in || ((t_token_lst *)redirs->content)->type == heredoc)
				if (make_redir_in((t_token_lst *)redirs->content))
					exit (EXIT_FAILURE);
			redirs = redirs->next;
		}
		redirs = NULL;
	}
}

int do_the_builtin(char **env[], char *cmd, char **cmd_tab)
{
	if (!ft_strcmp(cmd, "export"))
	{
		return (export(env, &cmd_tab[1]));
	}
	else if (!ft_strcmp(cmd, "pwd"))
	{
		pwd();
	}
	else if (!ft_strcmp(cmd, "env"))
	{
		return (print_env(*env));
	}
	else if (!ft_strcmp(cmd, "cd"))
	{
	   /*  if (more_than_one_arg)
			bash: cd: too many arguments
			exit(); */
		return (change_directory(cmd_tab[1], env));
	}
	else if (!ft_strcmp(cmd, "unset"))
	{
		return (unset(env, &cmd_tab[1]));
	}
	return (0);
}

int is_a_path(char *cmd)
{
	if (!access(cmd, F_OK))
		return (1);
	return (0);
}

char    *set_cmd_path(char *envp[], char *cmd)
{
	if (is_a_path(cmd))
		return (cmd);
	return (get_cmd_path(envp, cmd));
}

int exec_command(char **envp[], t_cmd_block *cmd_block)
{
	char *path;

	path = set_cmd_path(*envp, cmd_block->exec_tab[0]);
	execve(path, cmd_block->exec_tab, *envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void    restore_stds_and_close_dup(int out_save, int in_save)
{
	if  (out_save != -1)
	{
		dup2(out_save, STDOUT_FILENO);
		close(out_save);
	}
	if (in_save != -1)
	{
		dup2(in_save, STDIN_FILENO);
		close(in_save);
	}
}

int exec_not_builtin(t_cmd_block *cmd_block, char **envp[], int out_save, int in_save)
{
	pid_t   pid;
	int     status;

	pid = fork();
	if (pid == -1) 
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0) 
	{
		handle_redirs(cmd_block);
		exec_command(envp, cmd_block);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1) 
		{
			perror("waitpid");
			return (-1);
		}
		return (status);
	}
}

int exec_command_and_redirs(t_cmd_block *cmd_block, char **envp[])
{
	int     status;
	int     stdout_save;
	int     stdin_save;

	create_exec_tab(cmd_block);
	stdout_save = dup(STDOUT_FILENO);
	stdin_save = dup(STDIN_FILENO);
	if (!cmd_block->exec_tab[0])
	{
		handle_redirs(cmd_block);
		restore_stds_and_close_dup(stdout_save, stdin_save);
		return (0);
	}
	if (is_a_builtin(cmd_block->exec_tab[0]))
	{
		handle_redirs(cmd_block);
		status = do_the_builtin(envp, cmd_block->exec_tab[0], cmd_block->exec_tab);
		restore_stds_and_close_dup(stdout_save, stdin_save);
		return(status);
	}
	else
	{
		status = exec_not_builtin(cmd_block, envp, stdout_save, stdin_save);
		restore_stds_and_close_dup(stdout_save, stdin_save);
		return (status);
	}
	return (-1);
}

int	exec_ast(t_ast_node *ast, char **envp[])
{
	int ret_value;
	
	ret_value = 0;
	if (ast->type == pipe_op)
		return (handle_pipes(ast, envp));
	else if (ast->type == and_op)
	{
		ret_value = exec_ast(ast->left, envp);
		if (!ret_value)
			return (exec_ast(ast->right, envp));
		return (ret_value);
	}
	else if (ast->type == or_op)
	{
		ret_value = exec_ast(ast->left, envp);
		if (ret_value)
			ret_value = exec_ast(ast->right, envp);
		return (ret_value);
	}
	else
		return (exec_command_and_redirs(ast->cmd_block, envp));
}
