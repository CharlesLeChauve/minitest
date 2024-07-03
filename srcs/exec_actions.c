/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:26:38 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:10 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_a_builtin(char *command)
{
	if (!ft_strcmp(command, "cd") || !ft_strcmp(command, "echo")
		|| !ft_strcmp(command, "env") || !ft_strcmp(command, "exit")
		|| !ft_strcmp(command, "export") || !ft_strcmp(command, "pwd")
		|| !ft_strcmp(command, "unset"))
		return (1);
	return (0);
}

int	do_the_builtin(t_shell *shl, char **env[], char *cmd, char **cmd_tab)
{
	if (!ft_strcmp(cmd, "export"))
		return (export(env, &cmd_tab[1]));
	else if (!ft_strcmp(cmd, "echo"))
		ft_echo(cmd_tab);
	else if (!ft_strcmp(cmd, "pwd"))
		pwd();
	else if (!ft_strcmp(cmd, "env"))
		return (print_env(shl->env));
	else if (!ft_strcmp(cmd, "cd"))
		return (change_directory(&cmd_tab[1], env));
	else if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(cmd_tab, shl));
	else if (!ft_strcmp(cmd, "unset"))
		return (unset(env, &cmd_tab[1]));
	return (0);
}

int	wait_status(pid_t pid)
{
	int	status;
	int	ret_val;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (-1);
	}
	if (WIFEXITED(status))
	{
		ret_val = WEXITSTATUS(status);
		return (ret_val);
	}
	else
		return (-1);
}
