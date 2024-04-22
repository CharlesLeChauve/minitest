#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

// Fonction pour créer un nouveau nœud
t_ast_node *create_node(t_token_type type, char *text, t_token_lst *tokens) {
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    if (type == command && tokens != NULL)
        node->tokens = tokens;
    else
        node->tokens = NULL;
    node->value = ft_strdup(text);
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
t_ast_node *parse_tokens(t_token_lst *tokens) {
    if (tokens == NULL) return NULL;

    // Trouver le premier opérateur
    t_token_lst *current = tokens, *previous = NULL;
    while (current != NULL) {
        if (current->type == pipe_op || current->type == and_op || current->type == or_op) {
            break;
        }
        previous = current;
        current = current->next;
    }

    // Si aucun opérateur n'est trouvé, il s'agit d'une simple commande
    if (current == NULL) {
        return create_node(command, tokens->text, tokens);
    }

    // Créer le nœud pour l'opérateur
    t_ast_node *node = create_node(current->type, current->text, NULL);

    // Séparer la liste de jetons en deux parties
    t_token_lst *left_tokens = tokens;
    t_token_lst *right_tokens = current->next;

    // Important: exclure l'opérateur de la liste de jetons pour éviter la récursivité infinie
    if (previous != NULL) {
        previous->next = NULL; // Cela termine la liste de gauche au token précédant l'opérateur
    }

    // Construire les sous-arbres gauche et droit récursivement
    node->left = parse_tokens(left_tokens);
    node->right = parse_tokens(right_tokens);

    return node;
}

// Fonction de test pour afficher l'arbre syntaxique (à des fins de débogage)
void print_tree(t_ast_node *root) {
    t_token_lst *token_node;
    if (root == NULL) return;
    print_tree(root->left);
    printf("%s\n", root->value);
    if (root->tokens)
    {
    token_node = root->tokens;
			while (token_node)
			{
				print_token_type(token_node);
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
