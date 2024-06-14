/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:32:36 by tgibert           #+#    #+#             */
/*   Updated: 2024/06/13 14:20:09 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *extract_var_id(char *var)
{
	int i = 0;
	int	index = -1;

	while (var[i])
	{
		if (var[i] == '=')
		{
			index = i - 1;
			break ;
		}
		i++;
	}
	if (index > -1)
		return (ft_strndup(var, index + 1));
	return (var);
}

void	replace_var(char ***env, char *new_var, char *old_var)
{
	int	i;

	i = 0;
	while (ft_strcmp((*env)[i], old_var))
		i++;
	free((*env)[i]);
	(*env)[i] = ft_strdup(new_var);
}

void	replace_existing_vars(char ***arg, char ***env)
{
	int		i;
	char	*var;
	char	*var_id;

	i = 0;
	var = NULL;
	while ((*arg)[i])
	{
		var_id = extract_var_id((*arg)[i]);
		var = get_env_var(*env, var_id);
		if (var && !no_equal((*arg)[i]))
		{
			replace_var(env, (*arg)[i], var);
			remove_from_tab(arg, i);
		}
		else if (var)
			remove_from_tab(arg, i);
		else
			i++;
		var = NULL;
	}
}

char	*get_env_var(char **env, char *var_id)
{
	int		i;
	char	*env_var;
	int		id_len;

	id_len = ft_strlen(var_id);
	env_var = NULL;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var_id, id_len) && env[i][id_len] == '=')
		{
			env_var = env[i];
			break ;
		}
	}
	return (env_var);
}

int	get_env_index(char **env, char *var_id)
{
	int		i;
	char	*env_var;
	int		id_len;

	id_len = ft_strlen(var_id);
	env_var = NULL;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var_id, id_len))
		{
			env_var = env[i];
			break ;
		}
	}
	if (env_var)
		return (i);
	return (-1);
}
