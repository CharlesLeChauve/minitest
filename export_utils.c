/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:33:58 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 16:33:59 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_env(char **env)
{
	int	i;

	i = 0;
	ft_sort_wordtab(env);
	while (env[i])
	{
		if (strncmp(env[i], "_=", 2) == 0)
		{
			i++;
			continue ;
		}
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
}

char	*export_format(char *str)
{
	char	*formated;
	int		i;
	int		j;

	i = 0;
	j = 0;
	formated = (char *)malloc(ft_strlen(str) + 3);
	ft_bzero(formated, ft_strlen(str) + 3);
	while (str[i] && (i == 0 || (str[i - 1] != '=')))
		formated[j++] = str[i++];
	if (str[i] == '\0' && !(str[i - 1] == '='))
		return (formated);
	formated[j++] = '"';
	while (str[i])
	{
		formated[j] = str[i];
		i++;
		j++;
	}
	formated[j++] = '"';
	formated[j++] = '\0';
	return (formated);
}

int	export_err(char *arg)
{
	char	err[128];

	ft_bzero(err, 128);
	ft_sprintf(err, "tash: export: '%s': not a valid identifier\n", arg);
	ft_putstr_fd(err, STDERR_FILENO);
	return (1);
}

int	check_export_arg(char *arg)
{
	int		i;
	int		alpha;
	char	err[128];

	i = 0;
	alpha = 0;
	ft_bzero(err, 128);
	if (arg[0] == '=')
	{
		return (export_err(arg));
	}
	while (arg[i] && arg[i] != '=')
	{
		if ((!ft_isalnum(arg[i]) && arg[i] != '_') \
			|| (alpha == 0 && ft_isdigit(arg[i])))
			return (export_err(arg));
		if (ft_isalpha(arg[i]))
			alpha = 1;
		i++;
	}
	return (0);
}
