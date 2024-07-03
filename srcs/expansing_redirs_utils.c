/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing_redirs_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:46 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:14 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isrediroperator(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	set_quotes_state_in_redir(char **curr_char, t_sm *state, int *quotectrl)
{
	char	*start;

	start = *curr_char;
	if ((**curr_char == '"' || **curr_char == '\'') && *state == reg)
	{
		*quotectrl = 1;
		if (**curr_char == '"')
			*state = dquote;
		else
			*state = quote;
		(*curr_char)++;
	}
	else if (**curr_char == '"' && *state == dquote)
	{
		*state = reg;
		(*curr_char)++;
	}
	else if (**curr_char == '\'' && *state == quote)
	{
		*state = reg;
		(*curr_char)++;
	}
	if (start != *curr_char && (**curr_char == '"' || **curr_char == '\''))
		set_quotes_state_in_redir(curr_char, state, quotectrl);
}

t_token_type	redir_type(char *curr_char)
{
	if (*curr_char == '>')
	{
		curr_char++;
		if (*curr_char == '>')
		{
			curr_char++;
			return (redir_app);
		}
		else
			return (redir_out);
	}
	else if (*curr_char == '<')
	{
		curr_char++;
		if (*curr_char == '<')
		{
			curr_char++;
			return (heredoc);
		}
		else
			return (redir_in);
	}
	return (11);
}
