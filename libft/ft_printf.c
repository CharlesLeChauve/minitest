/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:00:35 by tgibert           #+#    #+#             */
/*   Updated: 2023/12/07 11:15:03 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	print_arg(va_list args, char c)
{
	if (c == 'c')
		return (printchar(va_arg(args, int)));
	if (c == 's')
		return (printstr(va_arg(args, char *)));
	if (c == 'i')
		return (ft_printnbr(va_arg(args, int)));
	if (c == 'd')
		return (ft_printnbr(va_arg(args, int)));
	if (c == '%')
		return (printchar('%'));
	if (c == 'u')
		return (ft_putunsigned(va_arg(args, unsigned int)));
	if (c == 'p')
		return (ft_putpointer(va_arg(args, void *)));
	if (c == 'x')
		return (ft_putlowhex(va_arg(args, int)));
	if (c == 'X')
		return (ft_putuphex(va_arg(args, int)));
	return (0);
}

int	ft_printf(const char *input, ...)
{
	va_list	args;
	size_t	printlen;
	size_t	i;

	va_start(args, input);
	i = 0;
	printlen = 0;
	while (input[i])
	{
		if (input[i] != '%')
			printlen += printchar(input[i]);
		if (input[i] == '%')
		{
			printlen += print_arg(args, input[i + 1]);
			i++;
		}
		i++;
	}
	va_end(args);
	return (printlen);
}
