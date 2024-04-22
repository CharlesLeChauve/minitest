/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 00:22:40 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/11 13:20:56 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcat(char *dest, const char *src)
{
	size_t	dstsize;
	int		i;

	i = 0;
	dstsize = ft_strlen(dest);
	if (!src || !dest)
		return ;
	while (src[i])
	{
		dest[dstsize + i] = src[i];
		i++;
	}
	dest[dstsize + i] = '\0';
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dstsize;
	size_t	addedsize;

	dstsize = ft_strlen(dest);
	addedsize = 0;
	if (size < 1)
		return (ft_strlen(src));
	if (size < dstsize)
		return (ft_strlen(src) + size);
	while (dstsize + addedsize < size - 1 && src[addedsize])
	{
		dest[dstsize + addedsize] = src[addedsize];
		addedsize++;
	}
	dest[dstsize + addedsize] = '\0';
	return (dstsize + ft_strlen(src));
}
