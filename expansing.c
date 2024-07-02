/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:52 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:02:57 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_command_option(char *token, t_cmd_block *block, t_shell *shell)
{
	char	*ptr;
	char	*sub_token;
	int		is_a_redir;

	is_a_redir = 0;
	ptr = token;
	while (*ptr)
	{
		while (ft_isspace(*ptr))
			ptr++;
		if (*ptr)
		{
			sub_token = extract_command(&ptr, shell, &is_a_redir);
			if (!sub_token)
			{
				if (!is_a_redir)
					continue ;
				else
					return (1);
			}
			if (process_sub_token(sub_token, block, is_a_redir))
				return (1);
		}
	}
	return (0);
}

int	fill_cmd_block(t_cmd_block *block, t_dlist *tokens, t_shell *shell)
{
	t_dlist		*current;
	t_token_lst	*token;
	char		*token_text;

	current = tokens;
	while (current)
	{
		token = (t_token_lst *)(current->content);
		token_text = token->text;
		if (token_text == NULL)
		{
			current = current->next;
			continue ;
		}
		if (parse_command_option(token_text, block, shell))
			return (1);
		current = current->next;
	}
	return (0);
}

int	expand_ast(t_ast_node *node, t_shell *shl)
{
	t_cmd_block	*cmd_block;

	if (node == NULL)
		return (1);
	if (node->type == command)
	{
		cmd_block = init_cmd_block();
		if (fill_cmd_block(cmd_block, node->tokens, shl))
			return (1);
		create_exec_tab(cmd_block);
		if (get_heredocs(cmd_block))
		{
			clear_cmd_block(cmd_block);
			return (1);
		}
		node->cmd_block = cmd_block;
	}
	expand_ast(node->left, shl);
	expand_ast(node->right, shl);
	return (0);
}

void	free_token_lst_content(void *content)
{
	t_token_lst	*token;

	token = (t_token_lst *)content;
	free(token->text);
}

void	clear_cmd_block(t_cmd_block *block)
{
	if (!block)
		return ;
	if (block->redirs && block->redirs->content)
		ft_lstiter(block->redirs, free_token_lst_content);
	if (block->arg)
		ft_lstclear(&(block->arg), free);
	if (block->redirs)
		ft_lstclear(&(block->redirs), free);
	if (block->exec_tab)
		ft_free_tab(block->exec_tab);
	free(block->command);
	free(block);
}
