/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:34:00 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:17 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**build_export_env(char **env[])
{
	int		i;
	char	**env_cpy;

	i = 0;
	while ((*env)[i])
		i++;
	env_cpy = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while ((*env)[++i])
		env_cpy[i] = export_format((*env)[i]);
	env_cpy[i] = NULL;
	return (env_cpy);
}

void	add_str_to_strtab(char *str, char ***strtab)
{
	int	env_size;
	int	total_size;

	env_size = 0;
	while ((*strtab)[env_size])
		env_size++;
	total_size = env_size + 1;
	*strtab = ft_realloc(*strtab, env_size * sizeof(char *), \
		(total_size + 1) * sizeof(char *));
	if (*strtab == NULL)
		return ;
	(*strtab)[env_size] = str;
	if ((*strtab)[env_size] == NULL)
		return ;
	(*strtab)[env_size + 1] = NULL;
}

int	export_one(char **arg, char **env[])
{
	char	*var_id;
	char	*var;

	var_id = NULL;
	if (check_export_arg(*arg))
		return (1);
	else
		var_id = extract_var_id(*arg);
	if (get_env_var(*env, var_id))
	{
		var = get_env_var(*env, var_id);
		replace_var(env, ft_strdup(*arg), var);
		var = NULL;
	}
	else
		add_str_to_strtab(ft_strdup(*arg), env);
	if (var_id)
		free(var_id);
	return (0);
}

int	export(char **env[], char **arg)
{
	char	**env_cpy;

	if (*arg == NULL)
	{
		env_cpy = build_export_env(env);
		print_export_env(env_cpy);
		ft_free_tab(env_cpy);
	}
	else
	{
		while (*arg)
		{
			if (export_one(arg, env))
				return (1);
			arg++;
		}
	}
	return (0);
}
