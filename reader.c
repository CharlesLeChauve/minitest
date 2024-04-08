/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:17:20 by tgibert           #+#    #+#             */
/*   Updated: 2024/03/21 07:15:28 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minitests.h"


t_syntax_node	*create_ast_node(t_type type, char **content)
{
	t_syntax_node *node = (t_syntax_node *)malloc(sizeof(t_syntax_node));
	if (!node)
	{
		perror("Allocation Failed");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->right = NULL;
	return (node);
}


void free_tree(t_syntax_node *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free_node(root);
    }
}

void free_node(t_syntax_node *node) {
    if (node) {
        if (node->content) {
            // Libérer chaque chaîne dans data, puis data lui-même
            char **temp = node->content;
            while (*temp) {
                free(*temp);
                temp++;
            }
            free(node->content);
        }
        free(node);
    }
}

t_syntax_node *tokenize_terms(char **terms)
{
	char **token_content;

	
}

int	main(void)
{
	char	*input;
	char	**terms;
	t_syntax_node **token_lst;				//In this one, every token is in line. the "right" pointer is here equal to a next;
	t_syntax_node **ast;

	while (1)
	{
		input = readline("Minishell > ");
		if (input && *input)
   			add_history(input);
		printf("You entered : %s\n", input);
		terms = ft_split(input, ' ');
		*token_lst = tokenize_terms(terms);
		free(input);
	}
}
