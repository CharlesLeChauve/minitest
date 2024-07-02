/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:03:00 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:03:00 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_node(t_token_type type, char *text, t_dlist *tokens)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (node == NULL)
		handle_memory_error();
	node->type = type;
	if (type == command || type == subshell)
		initialize_command_node(node, text, tokens);
	else
		initialize_operator_node(node);
	node->left = NULL;
	node->right = NULL;
	node->cmd_block = NULL;
	return (node);
}

t_ast_node	*create_subtree(t_dlist *op_node, t_dlist *tokens)
{
	t_token_type	type;
	t_ast_node		*node;
	t_dlist			*left_tokens;
	t_dlist			*right_tokens;

	type = ((t_token_lst *)(op_node->content))->type;
	del_tkn_node(op_node->content);
	left_tokens = tokens;
	node = create_node(type, NULL, NULL);
	right_tokens = op_node->next;
	if (op_node->prev)
		op_node->prev->next = NULL;
	free(op_node);
	node->left = parse_tokens(left_tokens);
	node->right = parse_tokens(right_tokens);
	return (node);
}

void	find_last_operators(t_dlist *tokens,
t_dlist **last_logical_op, t_dlist **last_op)
{
	t_dlist		*current;
	t_token_lst	*token;

	current = tokens;
	while (current != NULL)
	{
		token = (t_token_lst *)(current->content);
		if (token->type == and_op || token->type == or_op \
			|| token->type == pipe_op)
		{
			*last_op = current;
			if (token->type == and_op || token->type == or_op)
				*last_logical_op = current;
		}
		current = current->next;
	}
}

t_ast_node	*create_subtree_based_on_operators(t_dlist *last_logical_op,
t_dlist *last_op, t_dlist *tokens)
{
	t_token_lst	*first_token;

	if (last_logical_op != NULL)
		return (create_subtree(last_logical_op, tokens));
	else if (last_op != NULL)
		return (create_subtree(last_op, tokens));
	first_token = (t_token_lst *)(tokens->content);
	if (first_token->type == subshell)
		return (create_node(subshell, first_token->text, tokens));
	else
		return (create_node(command, first_token->text, tokens));
}

t_ast_node	*parse_tokens(t_dlist *tokens)
{
	t_dlist	*last_logical_op;
	t_dlist	*last_op;

	last_logical_op = NULL;
	last_op = NULL;
	if (tokens == NULL)
		return (NULL);
	find_last_operators(tokens, &last_logical_op, &last_op);
	return (create_subtree_based_on_operators(last_logical_op,
			last_op, tokens));
}
