/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:41 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:13 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_quotes_state_in_cmd_block(char **curr_char, t_sm *state)
{
	if ((**curr_char == '"' || **curr_char == '\'') && *state == reg)
	{
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
}

int	same_quote(t_sm *state, char c)
{
	if (*state == dquote && c == '"')
		return (1);
	else if (*state == quote && c == '\'')
		return (1);
	return (0);
}

void	handle_state(char **ptr, t_params *params,
char **buffer, t_shell *shell)
{
	char	*ext;

	ext = NULL;
	set_quotes_state_in_cmd_block(ptr, &(params->state));
	if (**ptr == '$' && params->state != quote)
	{
		ext = extrapolate(ptr, shell);
		ft_strappend(buffer, ext, &(params->len));
		if (ext)
			free(ext);
	}
	else if (same_quote(&(params->state), **ptr))
	{
		if (!*buffer)
			*buffer = ft_strdup("");
	}
}

char	*handle_redirection_or_not(char **ptr, t_params *params,
int *is_a_redir, char *buffer)
{
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	buffer = redir_token(ptr, &(params->state));
	*is_a_redir = 1;
	return (buffer);
}

char	*extract_command(char **ptr, t_shell *shell, int *is_a_redir)
{
	char		*buffer;
	t_params	params;

	params.len = 0;
	buffer = NULL;
	params.state = reg;
	while (**ptr)
	{
		handle_state(ptr, &params, &buffer, shell);
		if (**ptr == '$' && params.state != quote)
			continue ;
		else if (same_quote(&(params.state), **ptr))
			continue ;
		else if (!**ptr || (params.state == reg && ft_isspace(**ptr)))
			break ;
		else if (params.state == reg && (**ptr == '>' || **ptr == '<'))
			return (handle_redirection_or_not(ptr, &params, \
					is_a_redir, buffer));
		ft_add_char_to_buffer(&buffer, **ptr, &(params.len));
		(*ptr)++;
	}
	return (buffer);
}
