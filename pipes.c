#include "minishell.h"

void do_pipe_side(t_pipe_info *pipe_info, t_shell *shl, t_ast_node *ast, int side)
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
		dup2(pipe_info->pipe_fds[side ^ 1], side ^ 1);
		close(pipe_info->pipe_fds[side ^ 1]);
		if (side == 0)
			exit(exec_ast(ast->left, shl));
		else
			exit(exec_ast(ast->right, shl));
	}
}

void do_pipes(t_pipe_info *pipe_info, t_shell *shl, t_ast_node *ast)
{
	do_pipe_side(pipe_info, shl, ast, 0);
	do_pipe_side(pipe_info, shl, ast, 1);
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

int	handle_pipes(t_ast_node *ast, t_shell *shl)
{
	t_pipe_info	pipe_info;
	int			ret_value;

	init_pipe_info(&pipe_info);
	do_pipes(&pipe_info, shl, ast);
	waitpid(pipe_info.pids[0], NULL, 0);
	ret_value = wait_status(pipe_info.pids[1]);
	return (ret_value);
}
