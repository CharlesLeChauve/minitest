/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 07:07:46 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 07:56:48 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	exec_command(t_shell *shl, t_cmd_block *cmd_block)
{
	char		*path;
	struct stat	path_stat;

	path = NULL;
	if (path_start(cmd_block->exec_tab[0]) \
		&& stat(cmd_block->exec_tab[0], &path_stat) > -1 \
		&& S_ISDIR(path_stat.st_mode))
	{
		exec_error(cmd_block->exec_tab[0], 2);
		exit_exec(shl, 126);
	}
	else if (path_start(cmd_block->exec_tab[0]))
		exec_path_access(shl, cmd_block->exec_tab[0]);
	path = set_cmd_path(shl->env, cmd_block->exec_tab[0]);
	if (!path)
	{
		exec_error(cmd_block->exec_tab[0], 1);
		exit_exec(shl, 127);
	}
	if (execve(path, cmd_block->exec_tab, shl->env) == -1)
	{
		perror("execve");
		exit_exec(shl, 127);
	}
	return (0);
}

int	exec_not_builtin(t_cmd_block *cmd_block, t_shell *shl)
{
	pid_t	pid;
	int		ret_val;

	ret_val = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		exec_command(shl, cmd_block);
	else
		ret_val = wait_status(pid);
	return (ret_val);
}

int	cmd_and_restore(t_cmd_block *cmd_block, t_shell *shl, int fd_out, int fd_in)
{
	int	status;

	status = exec_not_builtin(cmd_block, shl);
	restore_stds_and_close_dup(fd_out, fd_in, -1);
	return (status);
}

int	exec_command_and_redirs(t_cmd_block *cmd_block, t_shell *shl)
{
	int				status;
	t_std_fd_save	save;

	save.std_out = dup(STDOUT_FILENO);
	save.std_in = dup(STDIN_FILENO);
	if (!cmd_block)
		return (130);
	status = handle_redirs(cmd_block);
	if (status || !cmd_block->exec_tab[0])
	{
		restore_stds_and_close_dup(save.std_out, save.std_in, -1);
		return (status);
	}
	if (is_a_builtin(cmd_block->exec_tab[0]))
	{
		status = do_the_builtin(shl, &(shl->env), \
			cmd_block->exec_tab[0], cmd_block->exec_tab);
		restore_stds_and_close_dup(save.std_out, save.std_in, -1);
		return (status);
	}
	else
		return (cmd_and_restore(cmd_block, shl, save.std_out, save.std_in));
	return (-1);
}

int	exec_ast(t_ast_node *ast, t_shell *shl)
{
	if (!ast)
		return (1);
	if (ast->type == subshell)
		return (ft_subshell(((t_token_lst *)(ast->tokens->content))->text, \
			shl->env));
	else if (ast->type == pipe_op)
		return (handle_pipes(ast, shl));
	else if (ast->type == and_op)
	{
		shl->last_ret = exec_ast(ast->left, shl);
		if (!shl->last_ret)
			shl->last_ret = exec_ast(ast->right, shl);
		return (shl->last_ret);
	}
	else if (ast->type == or_op)
	{
		shl->last_ret = exec_ast(ast->left, shl);
		if (shl->last_ret)
			shl->last_ret = exec_ast(ast->right, shl);
		return (shl->last_ret);
	}
	else
		return (exec_command_and_redirs(ast->cmd_block, shl));
}
