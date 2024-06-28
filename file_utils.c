/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:18:16 by tgibert           #+#    #+#             */
/*   Updated: 2024/06/28 09:08:07 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_acces(char *file, t_open_mode mode)
{
	char err[128];

	ft_bzero(err, 128);
	if ((mode == truncate_o || mode == append_o) && access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_sprintf(err, "permission denied : %s\n", file);
		ft_putstr_fd(err, 2);
		return (0);
	}
	else if (file[0] == 0 || (mode == read_o && access(file, F_OK) != 0))
	{
		ft_sprintf(err, "no such file or directory : %s\n", file);
		ft_putstr_fd(err, 2);
		return (0);
	}
	if (mode == read_o && access(file, R_OK) != 0)
	{
		ft_sprintf(err, "permission denied : %s\n", file);
		ft_putstr_fd(err, 2);
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