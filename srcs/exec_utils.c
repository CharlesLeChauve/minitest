/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:10 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:10 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_path_access(t_shell *shl, char *cmd)
{
	int			access;

	access = check_acces(cmd, read_o);
	if (access == 2)
	{
		ft_free_tab(shl->env);
		clean_shell_instance(shl);
		exit (126);
	}
	else if (access == 1)
	{
		ft_free_tab(shl->env);
		clean_shell_instance(shl);
		exit (127);
	}
}

void	exit_exec(t_shell *shl, int ex_no)
{
	ft_free_tab(shl->env);
	clean_shell_instance(shl);
	exit (ex_no);
}

void	exec_error(char *str, int mode)
{
	char		err[124];

	ft_bzero(err, 124);
	if (mode == 1)
	{
		ft_sprintf(err, "tash: %s: command not found\n", str);
		ft_putstr_fd(err, STDERR_FILENO);
	}
	if (mode == 2)
	{
		ft_sprintf(err, "tash: %s: Is a directory\n", str);
		ft_putstr_fd(err, STDERR_FILENO);
	}
}
