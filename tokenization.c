/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulece <tulece@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:03:29 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:08:39 by tulece           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_lst	*next_token(t_tkn_info *tkn_info)
{
	space_quotes(tkn_info);
	if (!*tkn_info->curr_char)
		return (token_new(eol, NULL));
	if (*tkn_info->curr_char == '(')
		return (handle_paren(tkn_info));
	if (*tkn_info->curr_char == '|' && tkn_info->state == reg)
		return (handle_pipe(tkn_info));
	if (*tkn_info->curr_char == '&' && tkn_info->state == reg)
		return (handle_ampersand(tkn_info));
	return (cmd_token(tkn_info));
}

t_token_lst	*token_new(t_token_type type, void *content)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = type;
	if (content == NULL)
	{
		token->text = NULL;
		return (token);
	}
	else
		token->text = ft_strdup((const char *)content);
	return (token);
}

void	handle_error_and_cleanup(t_tkn_info *tkn_info, const char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
	free(tkn_info->input);
	ft_dlstclear(&tkn_info->token_lst, del_tkn_node);
}

int	add_new_token(t_tkn_info *tkn_info)
{
	t_token_lst	*new_token;

	new_token = next_token(tkn_info);
	if (!new_token)
	{
		handle_error_and_cleanup(tkn_info, "Syntax error: invalid token.\n");
		return (0);
	}
	ft_dlstadd_back(&tkn_info->token_lst, ft_dlstnew(new_token));
	return (1);
}

t_dlist	*tokenize(char *input)
{
	t_tkn_info	tkn_info;
	t_dlist		*last;

	if (!input || !*input)
		return (NULL);
	init_tkn_info(&tkn_info, input);
	if (!tkn_info.input)
	{
		ft_printf("Error: Memory allocation failed\n");
		return (NULL);
	}
	if (!add_new_token(&tkn_info))
		return (NULL);
	last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
	while (last && ((t_token_lst *)(last->content))->type != eol)
	{
		if (!add_new_token(&tkn_info))
			return (NULL);
		last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
	}
	free(tkn_info.input);
	return (tkn_info.token_lst);
}
