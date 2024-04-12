/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:15:10 by tgibert           #+#    #+#             */
/*   Updated: 2024/02/27 14:49:15 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../libft.h"

int	check_nl(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	reset_buffer(char *buffer)
{
	unsigned long	i;
	unsigned long	len;

	i = 0;
	len = ft_strlen(buffer);
	while (i <= len)
	{
		buffer[i] = '\0';
		i++;
	}
}

int	read_file(char **stash, int fd)
{
	char	*buffer;
	int		nb_read;

	buffer = ft_strnew(BUFFER_SIZE);
	nb_read = 1;
	while (nb_read > 0 && !check_nl(*stash))
	{
		*stash = ft_gnljoin(*stash, buffer);
		reset_buffer(buffer);
		if (!check_nl(*stash))
			nb_read = read(fd, buffer, BUFFER_SIZE);
		if (nb_read == -1)
		{
			free(buffer);
			buffer = NULL;
			return (0);
		}
	}
	if (buffer)
		free(buffer);
	if ((*stash)[0] == 0)
	{
		return (0);
	}
	return (1);
}

char	*define_line(char **stash)
{
	char	*line;
	int		i;
	char	*new_stash;

	i = 0;
	if (ft_strlen(*stash) == 0)
		return (NULL);
	while ((*stash)[i] != '\n' && (*stash)[i])
		i++;
	line = ft_substr(*stash, 0, i + 1);
	new_stash = ft_substr(*stash, i + 1, ft_strlen(*stash) - i);
	free(*stash);
	*stash = new_stash;
	new_stash = NULL;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
	{
		stash = ft_strdup("");
	}
	if (check_nl(stash))
	{
		return (define_line(&stash));
	}
	else if (!read_file(&stash, fd))
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (define_line(&stash));
}
