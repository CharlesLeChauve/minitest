/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:03:49 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 14:03:50 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_option(char *arg)
{
	int	j;

	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

int	handle_options(char **args, int *newline)
{
	int	i;

	i = 1;
	*newline = 1;
	while (args[i] && args[i][0] == '-')
	{
		if (is_valid_option(args[i]))
		{
			*newline = 0;
			i++;
		}
		else
			break ;
	}
	return (i);
}

void	print_arguments(char **args, int i, int newline)
{
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = handle_options(args, &newline);
	print_arguments(args, i, newline);
}
