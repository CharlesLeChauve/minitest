/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_pattern.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 07:50:20 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:19 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_asterisk(char **pattern)
{
	while (**pattern == '*')
		(*pattern)++;
	return (!**pattern);
}

int	match_char(char **str, char **pattern, char **s_ptr, char **p_ptr)
{
	if (**pattern == '?' || **pattern == **str)
	{
		(*str)++;
		(*pattern)++;
	}
	else if (*s_ptr)
	{
		(*s_ptr)++;
		*str = *s_ptr;
		*pattern = *p_ptr;
	}
	else
		return (0);
	return (1);
}

int	match_pattern(char *str, char *pattern)
{
	char	*s_ptr;
	char	*p_ptr;

	p_ptr = NULL;
	s_ptr = NULL;
	if (str[0] == '.' && pattern[0] != '.')
		return (0);
	while (*str)
	{
		if (*pattern == '*')
		{
			if (handle_asterisk(&pattern))
				return (1);
			s_ptr = str;
			p_ptr = pattern;
		}
		else if (!match_char(&str, &pattern, &s_ptr, &p_ptr))
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern);
}
