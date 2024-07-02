/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_setup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulece <tulece@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:03:20 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 22:45:01 by tulece           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_tkn_info(t_tkn_info *tkn_info, char *input)
{
	tkn_info->input = ft_strdup(input);
	tkn_info->curr_char = tkn_info->input;
	tkn_info->token_lst = NULL;
	tkn_info->state = reg;
}

int	break_token(t_tkn_info *tkn_info)
{
	first_read_quotes(tkn_info);
	if ((tkn_info->state == reg && ft_isshelloperator(*tkn_info->curr_char)))
		return (1);
	else
		return (0);
}

void	set_token_text(t_tkn_info *tkn_info, t_token_lst *token)
{
	size_t		len;
	char		*buffer;

	len = 0;
	buffer = NULL;
	while (*tkn_info->curr_char)
	{
		if (tkn_info->curr_char != tkn_info->input && break_token(tkn_info))
			break ;
		ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len);
		tkn_info->curr_char++;
	}
	while (len > 0 && ft_isspace(buffer[len - 1]))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	token->text = buffer;
}

t_token_lst	*cmd_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = command;
	set_token_text(tkn_info, token);
	return (token);
}
