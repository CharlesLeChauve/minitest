/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:29:18 by tgibert           #+#    #+#             */
/*   Updated: 2024/02/27 14:50:22 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../libft.h"

char	*ft_strnew(unsigned long len)
{
	char				*new;
	unsigned long		i;

	i = 0;
	new = (char *)malloc(len + 1);
	if (new == NULL)
		return (NULL);
	while (i < len + 1)
		new[i++] = '\0';
	return (new);
}

char	*ft_gnljoin(char *stash, char *buffer)
{
	unsigned long		size;
	char				*result;
	unsigned long		i;
	unsigned long		j;

	i = 0;
	j = ft_strlen(stash);
	size = ft_strlen(stash) + ft_strlen(buffer);
	result = ft_strnew(size);
	if (result == NULL)
		return (NULL);
	while (i < j)
	{
		result[i] = stash[i];
		i++;
	}
	free(stash);
	j = 0;
	while (j < ft_strlen(buffer))
	{
		result[i + j] = buffer[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}
