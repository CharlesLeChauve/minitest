/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:23 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/09 13:35:53 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	getsize(int n)
{
	size_t	size;

	size = 1;
	while (n / 10 != 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	result[12];
	size_t	size;
	long	nb;

	nb = n;
	size = getsize(nb) + 1;
	if (nb < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	else
		size--;
	result[size] = '\0';
	while (size > 0)
	{
		size--;
		if (n < 0 && size == 0)
			break ;
		result[size] = (nb % 10) + '0';
		nb /= 10;
	}
	return (ft_strdup(result));
}
