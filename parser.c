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

void	free_tree(t_ast_node *root)
{
	if (root == NULL)
		return ;
	free_tree(root->left);
	free_tree(root->right);
	free(root->value);
	free(root);
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

// void	print_logical_node(t_ast_node *node)
// {
// 	switch (node->type)
// 	{
// 		case pipe_op:
// 			printf("Type: pipe |\t\n");
// 			break;
// 		case or_op:
// 			printf("Type: or ||\t\n");
// 			break;
// 		case and_op:
// 			printf("Type: and &&\t\n");
// 			break;
// 		default:
// 			printf("Bwaaaargh\t\n");
// 			break;
// 	}
// }

int	is_logical(t_token_type type)
{
	if (type == pipe_op || type == or_op || type == and_op)
		return (1);
	return (0);
}

// void	print_tree(t_ast_node *node, int depth)
// {
// 	char	*indent;
// 	int		i;

// 	i = -1;
// 	if (node == NULL)
// 		return ;
// 	indent = malloc(depth * 4 + 1);
// 	while (++i < depth * 4)
// 		indent[i] = ' ';
// 	indent[depth * 4] = '\0';
// 	if (is_logical(node->type))
// 		printf("%sLogical Node (%s)\n", indent, node->value);
// 	else if (node->type == command)
// 	{
// 		printf("%sCommand: [%s]\n", indent, node->value);
// 		print_cmd_block(node->cmd_block);
// 	}
// 	else
// 		printf("%sOther Node: [%s]\n", indent, node->value ? node->value : "NULL");
// 	// if (node->tokens)
// 	// {
// 	// 	token_node = node->tokens;
// 	// 	printf("%sTokens:\n", indent);
// 	// 	while (token_node)
// 	// 	{
// 	// 		printf("%s - %s\n", indent, ((t_token_lst *)token_node->content)->text);
// 	// 		print_token_type((t_token_lst *)token_node->content);
// 	// 		token_node = token_node->next;
// 	// 	}
// 	// }
// 	if (node->left)
// 	{
// 		printf("%sGauche:\n", indent);
// 		print_tree(node->left, depth + 1);
// 	}
// 	if (node->right)
// 	{
// 		printf("%sDroite:\n", indent);
// 		print_tree(node->right, depth + 1);
// 	}
// 	free(indent);
// }

// void print_ast(t_ast_node *root)
// {
// 	printf("AST Structure:\n");
// 	print_tree(root, 0);
// }
