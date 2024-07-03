/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:03:38 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:34 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_text(char *text)
{
	if (!text)
		return (1);
	while (*text)
	{
		if (*text != '(' && *text != ')' && !ft_isspace(*text))
			return (0);
		text++;
	}
	return (1);
}

int	check_sub_syntax(t_dlist *current, t_token_lst *token)
{
	char	err[128];

	ft_bzero(err, 128);
	if (!token->text || no_text(token->text))
	{
		ft_putstr_fd("tash: syntax error near unexpected token `)'\n", 2);
		return (1);
	}
	if (current->prev && \
		!is_logical(((t_token_lst *)(current->prev->content))->type))
	{
		ft_sprintf(err, "tash: syntax error near unexpected token `%s'\n", \
			((t_token_lst *)(current->content))->text);
		ft_putstr_fd(err, 2);
		return (1);
	}
	return (0);
}

int	check_op_syntax(t_dlist *current, t_token_lst *token)
{
	char	err[128];

	ft_bzero(err, 128);
	if (!current->prev)
	{
		ft_sprintf(err, "tash: syntax error near unexpected token `%s'\n", \
			token->text);
		ft_putstr_fd(err, 2);
		return (1);
	}
	if (!current->next)
	{
		ft_putstr_fd("tash: syntax error near unexpected token `newline'\n", 2);
		return (1);
	}
	if (current->next && ((t_token_lst *)(current->next->content))->type \
		!= command && ((t_token_lst *)(current->next->content))->type \
		!= subshell)
	{
		ft_sprintf(err, "tash: syntax error near unexpected token `%s'\n", \
			((t_token_lst *)(current->next->content))->text);
		ft_putstr_fd(err, 2);
		return (1);
	}
	return (0);
}

int	verify_token(t_dlist *current, t_token_lst *token)
{
	if (is_operator(token->type) && !is_redir(token->type))
	{
		if (check_op_syntax(current, token))
			return (0);
	}
	else if (token->type == subshell)
		if (check_sub_syntax(current, token))
			return (0);
	return (1);
}

int	verify_tokens(t_dlist *tokens)
{
	t_dlist		*current;
	t_token_lst	*token;
	int			ret;

	current = tokens;
	if (!tokens)
		return (0);
	while (current != NULL)
	{
		token = (t_token_lst *)current->content;
		ret = verify_token(current, token);
		if (ret == 0 || ret == -1)
			return (ret);
		current = current->next;
	}
	return (1);
}
