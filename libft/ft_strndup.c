/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:06:17 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 15:06:20 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	new_str = (char *)malloc(len + 1);
	if (new_str == NULL)
		return (NULL);
	new_str[len] = '\0';
	return ((char *)ft_memcpy(new_str, s, len));
}
