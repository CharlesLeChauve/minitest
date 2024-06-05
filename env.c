/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:25:38 by tgibert           #+#    #+#             */
/*   Updated: 2024/06/05 10:06:51 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	no_equal(char *var)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	while (var[i])
	{
		if (var[i] == '=')
			equal = 1;
		i++;
	}
	if (equal == 0)
		return (1);
	return (0);
}

int	print_env(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (no_equal(env[i]))
		{
			i++;
			continue ;
		}
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	unset(char ***env, char **args)
{
	char	**new_env;
	int		i;
	int		env_idx;

	i = 0;
	env_idx = 0;
	while (args[i])
	{
		env_idx = get_env_index(*env, args[i]);
		if (env_idx != -1)
		{
			remove_from_tab(env, env_idx);
			remove_from_tab(&args, i);
		}
		else
			i++;
	}
	return (0);
}
