/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:03:27 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:32 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isshelloperator(char c)
{
	if (c == '|' || c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

void	ft_add_char_to_buffer(char **buffer, char c, size_t *len)
{
	char	*new_buffer;

	new_buffer = malloc(*len + 2);
	if (!new_buffer)
	{
		ft_printf("Error: Allocation failed\n");
		exit(EXIT_FAILURE);
	}
	if (*buffer)
		ft_memcpy(new_buffer, *buffer, *len);
	new_buffer[*len] = c;
	(*len)++;
	new_buffer[*len] = '\0';
	free(*buffer);
	*buffer = new_buffer;
}

void	set_quotes_state(t_tkn_info *tkn_info)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') \
	&& tkn_info->state == reg)
	{
		if (*tkn_info->curr_char == '"')
			tkn_info->state = dquote;
		else
			tkn_info->state = quote;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
	{
		tkn_info->state = reg;
	}
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
		tkn_info->state = reg;
}

void	first_read_quotes(t_tkn_info *tkn_info)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') \
	&& tkn_info->state == reg)
	{
		if (*tkn_info->curr_char == '"')
			tkn_info->state = dquote;
		else
			tkn_info->state = quote;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
		tkn_info->state = reg;
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
		tkn_info->state = reg;
}

void	space_quotes(t_tkn_info *tkn_info)
{
	if (tkn_info->state == reg)
		while (ft_isspace(*(tkn_info->curr_char)))
			tkn_info->curr_char++;
	set_quotes_state(tkn_info);
}
