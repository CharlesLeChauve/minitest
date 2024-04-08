/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numbers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:50:35 by tgibert           #+#    #+#             */
/*   Updated: 2023/12/07 11:01:12 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	ft_printnbr(int n)
{
	char		digit;
	long long	nb;
	int			i;

	nb = (long long) n;
	i = 0;
	if (nb < 0)
	{
		nb *= -1;
		write(1, "-", 1);
		i++;
	}
	if (nb > 9)
	{
		i += ft_printnbr(nb / 10);
		i += ft_printnbr(nb % 10);
	}
	if (nb < 10)
	{
		digit = nb + '0';
		write(1, &digit, 1);
		i++;
	}
	return (i);
}

int	ft_putunsigned(unsigned int nb)
{
	char	digit;
	int		i;

	i = 0;
	if (nb > 9)
	{
		i += ft_putunsigned(nb / 10);
		i += ft_putunsigned(nb % 10);
	}
	if (nb < 10)
	{
		digit = nb + '0';
		write(1, &digit, 1);
		i++;
	}
	return (i);
}

int	ft_putlowhex(unsigned int n)
{
	char	*base;
	char	hexnb[17];
	int		i;
	int		digits;

	i = 15;
	digits = 0;
	base = "0123456789abcdef";
	hexnb[16] = '\0';
	if (n == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	while (n > 0)
	{
		hexnb[i] = base[n % 16];
		n /= 16;
		i--;
		digits++;
	}
	i++;
	printstr(&hexnb[i]);
	return (digits);
}

int	ft_putuphex(unsigned int n)
{
	char	*base;
	char	hexnb[17];
	int		i;
	int		digits;

	i = 15;
	digits = 0;
	base = "0123456789ABCDEF";
	hexnb[16] = '\0';
	if (n == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	while (n > 0)
	{
		hexnb[i] = base[n % 16];
		n /= 16;
		i--;
		digits++;
	}
	i++;
	printstr(&hexnb[i]);
	return (digits);
}
