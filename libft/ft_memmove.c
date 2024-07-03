/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:16 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/09 11:23:26 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest_ptr;
	unsigned char	*src_ptr;

	dest_ptr = (unsigned char *) dest;
	src_ptr = (unsigned char *) src;
	if (!dest_ptr && !src_ptr)
		return (NULL);
	if (src_ptr < dest_ptr && src_ptr + n > dest_ptr)
	{
		i = n;
		while (i--)
			dest_ptr[i] = src_ptr[i];
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
