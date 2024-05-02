#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"


// Fonction pour créer un nouveau nœud
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
        node->value = ft_strdup(text);
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

// Fonction pour libérer la mémoire de l'arbre
void free_tree(t_ast_node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->value);
    free(root);
}

// Fonction récursive pour construire l'arbre syntaxique
t_ast_node *parse_tokens(t_dlist *tokens) {
    if (tokens == NULL) return NULL;

    // Trouver le premier opérateur
    t_dlist *current = tokens, *previous = NULL;
    while (current != NULL) {
        if (((t_token_lst *)(current->content))->type == pipe_op || ((t_token_lst *)(current->content))->type == and_op || ((t_token_lst *)(current->content))->type == or_op) {
            break;
        }
        previous = current;
        current = current->next;
    }

    // Si aucun opérateur n'est trouvé, il s'agit d'une simple commande
    if (current == NULL) {
        return create_node(command, ((t_token_lst *)(tokens->content))->text, tokens);
    }

    // Créer le nœud pour l'opérateur
    t_ast_node *node = create_node(((t_token_lst *)(current->content))->type, ((t_token_lst *)(current->content))->text, NULL);

    // Séparer la liste de jetons en deux parties
    t_dlist *left_tokens = tokens;
    t_dlist *right_tokens = current->next;

    // Important: exclure l'opérateur de la liste de jetons pour éviter la récursivité infinie
    if (previous != NULL) {
        previous->next = NULL; // Cela termine la liste de gauche au token précédant l'opérateur
    }

    // Construire les sous-arbres gauche et droit récursivement
    node->left = parse_tokens(left_tokens);
    node->right = parse_tokens(right_tokens);

    return node;
}

void    print_logical_node(t_ast_node *node)
{
    switch (node->type) {
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
void print_tree(t_ast_node *root) {
    t_dlist *token_node;
    if (root == NULL) 
        return;
    print_tree(root->left);
    //printf("%s\n", root->value);
    if (is_logical(root->type))
        print_logical_node(root);
    if (root->tokens)
    {
    token_node = root->tokens;
			while (token_node)
			{
				print_token_type((t_token_lst *)token_node->content);
				token_node = token_node->next;
			}
    }
    print_tree(root->right);
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
