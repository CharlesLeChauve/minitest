/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:05:48 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:06:04 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strappend(char **dest, const char *src, size_t *len)
{
	size_t	src_len;
	char	*new_str;

	if (!src)
		return ;
	src_len = ft_strlen(src);
	if (*dest == NULL)
	{
		*dest = (char *)malloc(src_len + 1);
		if (!*dest)
			return ;
		ft_memcpy(*dest, src, src_len + 1);
		*len = src_len;
	}
	else
	{
		new_str = (char *)realloc(*dest, *len + src_len + 1);
		if (!new_str)
			return ;
		*dest = new_str;
		ft_memcpy(*dest + *len, src, src_len + 1);
		*len += src_len;
	}
}
