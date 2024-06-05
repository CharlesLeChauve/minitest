#include "minishell.h"

int exec_command(char **envp[], t_cmd_block *cmd_block)
{
	char *path;

	path = set_cmd_path(*envp, cmd_block->exec_tab[0]);
	execve(path, cmd_block->exec_tab, *envp);
	perror("execve");
	exit(EXIT_FAILURE);
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
