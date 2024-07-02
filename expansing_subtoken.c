/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansing_subtoken.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:02:50 by anporced          #+#    #+#             */
/*   Updated: 2024/07/02 17:02:51 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_block	*init_cmd_block(void)
{
	t_cmd_block	*block;

	block = malloc(sizeof(t_cmd_block));
	if (!block)
		return (NULL);
	block->exec_tab = NULL;
	block->command = NULL;
	block->arg = NULL;
	block->redirs = NULL;
	return (block);
}

int	expand_redir(char **str)
{
	char	*suffix;
	char	*prefix;
	int		index;

	index = get_last_full_dir_idx(*str);
	if (index >= 0)
	{
		prefix = ft_strndup(*str, index);
		suffix = ft_strdup(*str + index);
		if (simple_expand(prefix, suffix, str))
			return (1);
		free(prefix);
		prefix = NULL;
		free (suffix);
		suffix = NULL;
	}
	return (0);
}

int	handle_redirection(char *sub_token, t_cmd_block *block, t_token_type type)
{
	char	*str;
	t_list	*new_arg;

	if (type == heredoc || type == redir_app)
		str = ft_strdup(sub_token + 2);
	else if (type == redir_out || type == redir_in)
		str = ft_strdup(sub_token + 1);
	free(sub_token);
	if (type != heredoc && expand_redir(&str))
		return (1);
	new_arg = ft_lstnew(token_new(type, str));
	free(str);
	if (new_arg)
		ft_lstadd_back(&(block->redirs), new_arg);
	else
	{
		free(sub_token);
		sub_token = NULL;
	}
	return (0);
}

int	add_new_argument(char *sub_token, t_cmd_block *block)
{
	t_list	*new_arg;

	new_arg = ft_lstnew(sub_token);
	ft_lstadd_back(&(block->arg), new_arg);
	return (0);
}

int	process_sub_token(char *sub_token, t_cmd_block *block, int is_a_redir)
{
	t_token_type	type;

	if (is_a_redir && (sub_token[0] == '>' || sub_token[0] == '<'))
	{
		if (sub_token[1] && ((sub_token[1] == '<' && sub_token[0] == '>') \
		|| (sub_token[1] == '>' && sub_token[0] == '<')))
		{
			ft_putstr_fd("tash: Wrong redir operator\n", 2);
			return (1);
		}
		type = redir_type(sub_token);
		return (handle_redirection(sub_token, block, type));
	}
	else
		return (add_new_argument(sub_token, block));
}
