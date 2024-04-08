/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:35:10 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/04 01:21:19 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	n;
	void	*result;

	n = nmemb * size;
	result = malloc(n);
	if (result == NULL)
		return (NULL);
	ft_bzero(result, n);
	return (result);
}
