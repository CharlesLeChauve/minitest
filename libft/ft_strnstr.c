/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:45 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/04 01:39:37 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!(*needle))
		return ((char *)haystack);
	while (haystack[i] && i + j < n)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < n && needle[j])
			j++;
		if (needle[j] == 0)
			return ((char *) &haystack[i]);
		i++;
	}
	return (NULL);
}
