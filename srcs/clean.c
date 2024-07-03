/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:33:43 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:06 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	del_tkn_node(void *node_ptr)
{
	t_token_lst	*node;

	if (!node_ptr)
		return ;
	node = (t_token_lst *)node_ptr;
	if (node->text)
		free(node->text);
	node->text = NULL;
	free(node);
}

void	free_ast(t_ast_node *ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	clear_cmd_block(ast->cmd_block);
	ft_dlstclear(&(ast->tokens), del_tkn_node);
	if (ast->value != NULL)
		free(ast->value);
	free(ast);
}

void	destroy_heredocs(void)
{
	int		hd_1;
	int		hd_2;
	int		found;
	char	path[15];

	hd_1 = 0;
	while (1)
	{
		hd_2 = 0;
		ft_bzero(path, 15);
		ft_sprintf(path, ".heredoc%d.%d", hd_1, hd_2);
		found = 0;
		while (access(path, W_OK) == 0)
		{
			unlink(path);
			found++;
			hd_2++;
			ft_bzero(path, 15);
			ft_sprintf(path, ".heredoc%d.%d", hd_1, hd_2);
		}
		if (!found)
			break ;
		hd_1++;
	}
}

void	clean_shell_instance(t_shell *shl)
{
	if (shl->ast)
		free_ast(shl->ast);
	destroy_heredocs();
	shl->ast = NULL;
	shl->token_lst = NULL;
}
