/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing_extrapolation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:44 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:02:45 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_special_chars(char **str, t_shell *shell)
{
	if (**str == '?')
	{
		(*str)++;
		return (ft_itoa(shell->last_ret));
	}
	else if (**str == '$')
	{
		(*str)++;
		return (ft_itoa(getpid()));
	}
	return (NULL);
}

char	*get_env_value(char **str, t_shell *shell)
{
	char	*temp;
	char	*env_var_value;
	char	*value_start;
	size_t	j;

	j = 0;
	while ((*str)[j] && !ft_isspace((*str)[j]) && (*str)[j] != '$' \
			&& (*str)[j] != '\'' && (*str)[j] != '"' && (*str)[j] != '*')
		j++;
	temp = ft_substr(*str, 0, j);
	*str += j;
	env_var_value = get_env_var(shell->env, temp);
	free(temp);
	if (env_var_value)
	{
		value_start = ft_strchr(env_var_value, '=');
		if (value_start)
			return (ft_strdup(value_start + 1));
	}
	return (NULL);
}

char	*extrapolate(char **str, t_shell *shell)
{
	char	*result;

	(*str)++;
	if (!(**str) || (!ft_isalnum(**str) \
	&& (**str != '$' && **str != '?' && **str != '_')))
		return (ft_strdup("$"));
	result = handle_special_chars(str, shell);
	if (result)
		return (result);
	return (get_env_value(str, shell));
}
