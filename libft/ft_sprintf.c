/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:03:32 by tgibert           #+#    #+#             */
/*   Updated: 2023/12/21 06:54:24 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	sputstr(char *str, char *dst_str, int index)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_strcat(dst_str, "(null)");
		return (6);
	}
	while (str[i])
	{
		dst_str[index + i] = str[i];
		i++;
	}
	return (i);
}

int	sputnbr(int nbr, char *str, int index)
{
	char	*a_nbr;
	int		i;

	a_nbr = ft_itoa(nbr);
	i = 0;
	while (a_nbr[i])
	{
		str[index + i] = a_nbr[i];
		i++;
	}
	free(a_nbr);
	return (i);
}

int	sprint_arg(va_list args, char c, char *str, int index)
{
	if (c == 's')
		return (sputstr(va_arg(args, char *), str, index));
	if (c == 'd' || c == 'i')
		return (sputnbr(va_arg(args, int), str, index));
	return (0);
}

int	ft_sprintf(char *str, const char *input, ...)
{
	va_list	args;
	size_t	printlen;
	size_t	i;

	va_start(args, input);
	i = 0;
	printlen = 0;
	if (!input)
		return (-1);
	while (input[i])
	{
		if (input[i] == '%')
		{
			printlen += sprint_arg(args, input[i + 1], str, printlen);
			i++;
		}
		else
		{
			str[printlen] = input[i];
			printlen++;
		}
		i++;
	}
	va_end(args);
	return (printlen);
}
