/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:58 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:02:59 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_memory_error(void)
{
	fprintf(stderr, "Memory allocation failed\n");
	exit(EXIT_FAILURE);
}

void	initialize_command_node(t_ast_node *node, char *text, t_dlist *tokens)
{
	if (tokens == NULL)
	{
		free(node);
		return ;
	}
	node->tokens = tokens;
	if (text != NULL && text[0] != '\0')
	{
		node->value = ft_strdup(text);
		if (node->value == NULL)
			handle_memory_error();
	}
	else
		node->value = NULL;
}

void	initialize_operator_node(t_ast_node *node)
{
	node->tokens = NULL;
	if (node->type == pipe_op)
		node->value = ft_strdup("|");
	else if (node->type == and_op)
		node->value = ft_strdup("&&");
	else if (node->type == or_op)
		node->value = ft_strdup("||");
	if (node->value == NULL)
		handle_memory_error();
}
