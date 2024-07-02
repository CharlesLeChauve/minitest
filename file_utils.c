/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:18:16 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 13:53:24 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_acces(char *file, t_open_mode mode)
{
	char err[128];

	ft_bzero(err, 128);
	if ((mode == truncate_o || mode == append_o) && access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_sprintf(err, "tash: %s: Permission denied\n", file);
		ft_putstr_fd(err, 2);
		return (2);
	}
	else if (file[0] == 0 || (mode == read_o && access(file, F_OK) != 0))
	{
		ft_sprintf(err, "tash: %s: No such file or directory\n", file);
		ft_putstr_fd(err, 2);
		return (1);
	}
	if (mode == read_o && access(file, R_OK) != 0)
	{
		ft_sprintf(err, "tash: %s: Permission denied\n", file);
		ft_putstr_fd(err, 2);
		return (2);
	}
	return (0);
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
int	get_last_slash(char *ptr)
{
	int		idx;
	int		i;

	idx = -1;
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == '/')
			idx = i;
		i++;
	}
	return (idx);
}

int	open_write(char *file, t_open_mode mode)
{
	int	fd;
	int	i;
	char	err[128];
	char	*tmp_dir;

	i = get_last_slash(file);
	tmp_dir = NULL;
	ft_bzero(err, 128);
	if (i != -1)
		tmp_dir = ft_substr(file, 0, i + 1);
	if (tmp_dir)
	{
		if (access(tmp_dir, F_OK) == -1)
		{
			ft_printf("tmp = %s\n", tmp_dir);
			ft_sprintf(err, "tash: %s: No such file or directory\n", file);
			ft_putstr_fd(err, 2);
			free(tmp_dir);
			return (-2);
		}
		else if (access(tmp_dir, F_OK) == 0 && access(tmp_dir, W_OK) != 0)
		{
			ft_sprintf(err, "tash: %s: Permission denied\n", file);
			ft_putstr_fd(err, 2);
			free(tmp_dir);
			return(-2);
		}
		free(tmp_dir);
	}
	if (access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_sprintf(err, "tash: %s: Permission denied\n", file);
		ft_putstr_fd(err, 2);
		return (-2);
	}
	if (mode == truncate_o)
		fd = open_mode(file, truncate_o);
	else if (mode == append_o)
		fd = open_mode(file, append_o);
	else
		fd = -1;
	return (fd);
}
