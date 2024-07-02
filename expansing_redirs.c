/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:48 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:02:49 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes_and_operators(char **curr_char, t_sm *state,
char **buffer, t_params *params)
{
	set_quotes_state_in_redir(curr_char, state, &params->quote);
	if (**curr_char == '>' || **curr_char == '<')
	{
		ft_add_char_to_buffer(buffer, **curr_char, &params->len);
		(*curr_char)++;
	}
}

void	skip_whitespace(char **curr_char, t_sm *state, char **buffer,
size_t *len)
{
	while (**curr_char == ' ' || **curr_char == '\t')
	{
		if (*state != reg)
			ft_add_char_to_buffer(buffer, **curr_char, len);
		(*curr_char)++;
	}
}

int	handle_syntax_error(char **buffer, int quote)
{
	if (quote)
		return (1);
	ft_putstr_fd("tash : syntax error near unexpected token `newline'\n", 2);
	free(*buffer);
	return (0);
}

int	is_end_of_token(char c)
{
	return (ft_isrediroperator(c) || ft_isshelloperator(c) || c == '\0');
}

char	*redir_token(char **str, t_sm *state)
{
	char		*buffer;
	t_params	params;

	buffer = NULL;
	params.len = 0;
	params.quote = 0;
	handle_quotes_and_operators(str, state, &buffer, &params);
	handle_quotes_and_operators(str, state, &buffer, &params);
	skip_whitespace(str, state, &buffer, &params.len);
	if (is_end_of_token(**str) && !handle_syntax_error(&buffer, params.quote))
		return (NULL);
	else if (is_end_of_token(**str) \
	&& !handle_syntax_error(&buffer, params.quote))
		return (buffer);
	while (**str && !ft_isshelloperator(**str) && !ft_isrediroperator(**str))
	{
		set_quotes_state_in_redir(str, state, &params.quote);
		if (*state == reg && ft_isspace(**str))
			break ;
		ft_add_char_to_buffer(&buffer, **str, &params.len);
		(*str)++;
	}
	return (buffer);
}
