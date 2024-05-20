#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"


// Fonction pour créer un nouveau nœud
#include <string.h> // Pour strdup

t_ast_node *create_node(t_token_type type, char *text, t_dlist *tokens) {
	t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (node == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	if ((type == command || is_redir(type)) && tokens != NULL)
	{
		node->tokens = tokens;
		if (text != NULL && text[0] != '\0') {
			node->value = strdup(text);
			if (node->value == NULL) {
				fprintf(stderr, "Memory allocation failed\n");
				exit(EXIT_FAILURE);
			}
		} else {
			node->value = NULL; // Ou tout autre gestion appropriée pour une chaîne vide
		}
	}
	else
	{
		node->tokens = NULL;
		if (node->type == pipe_op)
			node->value = "|";
		if (node->type == and_op)
			node->value = "&&";
		if (node->type == or_op)
			node->value = "||";
	}
	node->left = NULL;
	node->right = NULL;
	return node;
}

// int operator_priority(t_token_type type) {
//     switch (type) {
//         case and_op:
//         case or_op:
//             return ();  // Higher priority
//         case pipe_op:
//             return ();    // Lower priority
//         default:
//             return 0;  // Not an operator, no priority
//     }
// }

// Fonction pour libérer la mémoire de l'arbre
void free_tree(t_ast_node *root) {
	if (root == NULL) return;
	free_tree(root->left);
	free_tree(root->right);
	free(root->value);
	free(root);
}


t_ast_node *create_subtree(t_dlist *op_node, t_dlist *tokens) {
	t_token_type type = ((t_token_lst *)(op_node->content))->type;
	char *text = ((t_token_lst *)(op_node->content))->text;

	t_ast_node *node = create_node(type, text, NULL);

	// Diviser les tokens en deux parties autour de l'opérateur
	t_dlist *left_tokens = tokens;
	t_dlist *right_tokens = op_node->next;
	if (op_node->prev) {
		op_node->prev->next = NULL; // Couper la liste à gauche de l'opérateur
	}

	// Construire les sous-arbres de manière récursive
	node->left = parse_tokens(left_tokens);
	node->right = parse_tokens(right_tokens);

	return node;
}


t_ast_node *parse_tokens(t_dlist *tokens)
{

	t_dlist	*current;
	t_dlist	*last_logical_op;
	t_dlist	*last_op;


	current = tokens;
	last_logical_op = NULL;
	last_op = NULL;
	if (tokens == NULL)
		return NULL;
	while (current != NULL)
	{
		if (((t_token_lst *)(current->content))->type == and_op ||
			((t_token_lst *)(current->content))->type == or_op ||
			((t_token_lst *)(current->content))->type == pipe_op) {
			last_op = current;
			if (((t_token_lst *)(current->content))->type == and_op ||
				((t_token_lst *)(current->content))->type == or_op)
				last_logical_op = current;
		}
		current = current->next;
	}

	if (last_logical_op != NULL)
		return create_subtree(last_logical_op, tokens);
	else if (last_op != NULL)
		return create_subtree(last_op, tokens);
	return (create_node(command, ((t_token_lst *)(tokens->content))->text, tokens));
}

void	print_logical_node(t_ast_node *node)
{
	switch (node->type)
	{
		case pipe_op:
			printf("Type: pipe |\t\n");
			break;
		case or_op:
			printf("Type: or ||\t\n");
			break;
		case and_op:
			printf("Type: and &&\t\n");
			break;
		default:
			printf("Bwaaaargh\t\n");
			break;
	}
}

int is_logical(t_token_type type)
{
	if (type == pipe_op || type == or_op || type == and_op)
		return (1);
	return (0);
}

// Fonction de test pour afficher l'arbre syntaxique (à des fins de débogage)
void	print_tree(t_ast_node *node, int depth)
{
	t_dlist	*token_node;
	char	*indent;
	int		i;

	i = -1;
	if (node == NULL)
		return ;
	indent = malloc(depth * 4 + 1);
	while (++i < depth * 4)
		indent[i] = ' ';
	indent[depth * 4] = '\0';
	if (is_logical(node->type))
		printf("%sLogical Node (%s)\n", indent, node->value);
	else if (node->type == command)
		printf("%sCommand: [%s]\n", indent, node->value);
	else
		printf("%sOther Node: [%s]\n", indent, node->value ? node->value : "NULL");
	if (node->tokens)
	{
		token_node = node->tokens;
		printf("%sTokens:\n", indent);
		while (token_node)
		{
			printf("%s - %s\n", indent, ((t_token_lst *)token_node->content)->text);
			token_node = token_node->next;
		}
	}
	if (node->left)
	{
		printf("%sGauche:\n", indent);
		print_tree(node->left, depth + 1);
	}
	if (node->right)
	{
		printf("%sDroite:\n", indent);
		print_tree(node->right, depth + 1);
	}
	free(indent);
}

// Fonction initiale pour commencer l'affichage à partir de la racine avec une profondeur de 0
void print_ast(t_ast_node *root)
{
	printf("AST Structure:\n");
	print_tree(root, 0);
}



// int ast_contructor() {
//     // Supposons que vous ayez une liste de jetons générés par le lexer
//     Token *tokens = (Token *)malloc(sizeof(Token));
//     tokens->text = "ls";
//     tokens->type = COMMAND;
//     tokens->next = (Token *)malloc(sizeof(Token));
//     tokens->next->text = "|";
//     tokens->next->type = PIPE;
//     tokens->next->next = (Token *)malloc(sizeof(Token));
//     tokens->next->next->text = "grep";
//     tokens->next->next->type = COMMAND;
//     tokens->next->next->next = NULL;

//     // Construire l'arbre syntaxique
//     t_ast_node *root = parse_tokens(tokens);

//     // Afficher l'arbre syntaxique (à des fins de débogage)
//     print_tree(root);

//     // Libérer la mémoire utilisée par l'arbre et les jetons
//     free_tree(root);

//     return 0;
// }
