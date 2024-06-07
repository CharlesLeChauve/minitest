#include "minishell.h"

int exec_command(char **envp[], t_cmd_block *cmd_block)
{
	char	*path;
	char	err_msg[124];
	struct stat	path_stat;

	ft_bzero(err_msg, 124);
	stat(cmd_block->exec_tab[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_sprintf(err_msg, "tash: %s: Is a directory\n", cmd_block->exec_tab[0]);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		exit (126);
	}
	path = set_cmd_path(*envp, cmd_block->exec_tab[0]);
	if (execve(path, cmd_block->exec_tab, *envp) == -1)
	{
		ft_sprintf(err_msg, "command not found: %s\n", cmd_block->exec_tab[0]);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		//ft_freetab(cmd_arr);
		exit (EXIT_FAILURE);
	}
}

int exec_not_builtin(t_cmd_block *cmd_block, char **envp[], int out_save, int in_save)
{
	pid_t   pid;
	int     status;
	int		ret_val;

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
		ret_val = wait_status(pid);
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
		status = handle_redirs(cmd_block);
		restore_stds_and_close_dup(stdout_save, stdin_save);
		return (status);
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

int	exec_ast(t_ast_node *ast, char **envp[], int *last_ret)
{
	expand_ast(ast, *last_ret, *envp);
	if (ast->type == pipe_op)
	{
		*last_ret = handle_pipes(ast, envp);
		return (*last_ret);
	}
	else if (ast->type == and_op)
	{
		*last_ret = exec_ast(ast->left, envp, last_ret);
		if (!*last_ret)
		{
			*last_ret = exec_ast(ast->right, envp, last_ret);
			return (*last_ret);
		}
		return (*last_ret);
	}
	else if (ast->type == or_op)
	{
		*last_ret = exec_ast(ast->left, envp, last_ret);
		if (*last_ret)
			*last_ret = exec_ast(ast->right, envp, last_ret);
		return (*last_ret);
	}
	else
		return (exec_command_and_redirs(ast->cmd_block, envp));
}
