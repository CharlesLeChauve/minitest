/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:51:30 by tgibert           #+#    #+#             */
/*   Updated: 2023/12/07 11:00:41 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	ft_putpointer(void *ptr)
{
	char			*base;
	char			hexnb[17];
	unsigned long	address;
	int				i;
	int				digits;

	address = (unsigned long) ptr;
	if (!ptr)
		return (printstr("(nil)"));
	i = 15;
	digits = 1;
	base = "0123456789abcdef";
	hexnb[16] = '\0';
	while (address > 0)
	{
		hexnb[i] = base[address % 16];
		address /= 16;
		i--;
		digits++;
	}
	i++;
	printstr("0x");
	printstr(&hexnb[i]);
	return (digits + 1);
}

int	printstr(const char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	else
	{
		while (str[i])
		{
			write(1, &str[i], 1);
			i++;
		}
	}
	return (i);
}

int	printchar(char c)
{
	write(1, &c, 1);
	return (1);
}
