/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:58 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:20 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_exec_tab(t_cmd_block *cmd_block)
{
	int		i;
	t_list	*current;

	expand_wildcards_in_block(cmd_block);
	i = ft_lstsize(cmd_block->arg);
	cmd_block->exec_tab = (char **)malloc((i + 2) * sizeof(char *));
	cmd_block->exec_tab[i + 1] = NULL;
	if (!cmd_block->exec_tab)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	current = cmd_block->arg;
	while (current)
	{
		cmd_block->exec_tab[i] = ft_strdup((char *)current->content);
		current = current->next;
		i++;
	}
	if (current && cmd_block->exec_tab[0])
		cmd_block->command = ft_strdup(cmd_block->exec_tab[0]);
	cmd_block->exec_tab[i] = NULL;
}

void	handle_memory_error(void)
{
	ft_putstr_fd("Memory allocation failed\n", STDERR_FILENO);
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
