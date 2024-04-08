/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:06:56 by tgibert           #+#    #+#             */
/*   Updated: 2023/12/07 10:54:49 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>

int	ft_putunsigned(unsigned int nb);
int	ft_putpointer(void *ptr);
int	ft_putlowhex(unsigned int n);
int	ft_putuphex(unsigned int n);
int	ft_printnbr(int n);
int	ft_printf(const char *input, ...);
int	printchar(char c);
int	printstr(const char *str);

#endif
