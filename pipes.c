#include "minishell.h"

void do_pipe_side(t_pipe_info *pipe_info, char **envp[], t_ast_node *ast, int side)
{
	pipe_info->pids[side] = fork();
	if (pipe_info->pids[side] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pipe_info->pids[side] == 0)
	{
		close(pipe_info->pipe_fds[side]);
		if (side == 0)
			dup2(pipe_info->pipe_fds[side ^ 1], STDOUT_FILENO);
		else
			dup2(pipe_info->pipe_fds[side ^ 1], STDIN_FILENO);
		close(pipe_info->pipe_fds[side ^ 1]);
		if (side == 0)
			exit(exec_ast(ast->left, envp));
		else
			exit(exec_ast(ast->right, envp));
	}
}

void do_pipes(t_pipe_info *pipe_info, char **envp[], t_ast_node *ast)
{
	do_pipe_side(pipe_info, envp, ast, 0);
	do_pipe_side(pipe_info, envp, ast, 1);
	close(pipe_info->pipe_fds[0]);
	close(pipe_info->pipe_fds[1]);
}

void	init_pipe_info(t_pipe_info *pipe_info)
{
	pipe_info->pipe_fds[0] = -1;
	pipe_info->pipe_fds[1] = -1;
	pipe_info->pids[0] = -1;
	pipe_info->pids[1] = -1;

	if (pipe(pipe_info->pipe_fds) == -1)
	{
	    perror("pipe");
	    exit(EXIT_FAILURE);
	}
}

int	handle_pipes(t_ast_node *ast, char **envp[])
{
	t_pipe_info	pipe_info;
	int			status;

	init_pipe_info(&pipe_info);
	do_pipes(&pipe_info, envp, ast);
	waitpid(pipe_info.pids[0], NULL, 0);
	waitpid(pipe_info.pids[1], &status, 0);
	return (status);
}
