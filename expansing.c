#include "minishell.h"

t_cmd_block	*init_cmd_block(void)
{
	t_cmd_block	*block;

	block = malloc(sizeof(t_cmd_block));
	if (!block)
		return (NULL);
	block->exec_tab = NULL;
	block->command = NULL;
	block->option = NULL;
	block->arg = NULL;
	block->redir_in = NULL;
	block->redir_out = NULL;
	return (block);
}

void	set_quotes_state_in_cmd_block(t_tkn_info *tkn_info)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') && tkn_info->state == reg)
	{
			if (*tkn_info->curr_char == '"')
				tkn_info->state = dquote;
			else
				tkn_info->state = quote;
		tkn_info->curr_char++;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
	{
		tkn_info->state = reg;
		tkn_info->curr_char++;
	}
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
	{
		tkn_info->state = reg;
		tkn_info->curr_char++;
	}
}

void	extract_command(char **ptr, t_cmd_block *block)
{
	char	*start = *ptr;

	while (**ptr && !ft_isspace(**ptr))
		(*ptr)++;
	if (*ptr > start)
		block->command = ft_strndup(start, *ptr - start);
}

char	*extract_sub_token(char **ptr)
{
	char	*start = *ptr;
	char	*token;
	int		in_quotes = 0;
	char	quote_char = '\0';
	size_t	len = 0;

	while (**ptr && (in_quotes || !ft_isspace(**ptr)))
	{
		if (**ptr == '"' || **ptr == '\'')
		{
			if (in_quotes && **ptr == quote_char)
				in_quotes = 0;
			else if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = **ptr;
			}
		}
		else
			len++;
		(*ptr)++;
	}
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	len = 0;
	while (start < *ptr)
	{
		if (*start != '"' && *start != '\'')
			token[len++] = *start;
		start++;
	}
	token[len] = '\0';
	return (token);
}

void	process_sub_token(char *sub_token, t_cmd_block *block)
{
	t_list	*new_opt;
	t_list	*new_arg;

	if (sub_token[0] == '-')
	{
		new_opt = ft_lstnew(sub_token);
		ft_lstadd_back(&(block->option), new_opt);
	}
	else if (sub_token[0] == '>' || sub_token[0] == '<')
	{
		new_arg = ft_lstnew(redir_token(sub_token));
		if (sub_token[0] == '>')
			ft_lstadd_back(&(block->redir_out), new_arg);
		if (sub_token[0] == '<')
			ft_lstadd_back(&(block->redir_in), new_arg);
	}
	else
	{
		new_arg = ft_lstnew(sub_token);
		ft_lstadd_back(&(block->arg), new_arg);
	}
}

void	parse_command_option(char *token, t_cmd_block *block)
{
	char	*ptr = token;
	char	*sub_token;
	int		command_found;

	command_found = 0;
	// if (*ptr == '>' || *ptr == '<')
	// {
	// 	sub_token = extract_sub_token(&ptr);
	// 	process_sub_token(sub_token, block);
	// 	free(sub_token);
	// }
	// extract_command(&ptr, block);
	while (*ptr)
	{
		while (ft_isspace(*ptr))
			ptr++;
		if (*ptr)
		{
			if (!command_found && *ptr != '>' && *ptr != '<')
			{
				extract_command(&ptr, block);
				command_found = 1;
			}
			else
			{
				sub_token = extract_sub_token(&ptr);
				process_sub_token(sub_token, block);
			}
		}
	}
}

void	fill_cmd_block(t_cmd_block *block, t_dlist *tokens)
{
	t_dlist		*current;
	t_list		*new_redir;
	t_list		*redir;
	t_token_lst	*token;
	t_list		*opt;
	t_list		*arg;
	char		*token_text;

	current = tokens;
	while (current)
	{
		token = (t_token_lst *)(current->content);
		token_text = token->text;
		if (token_text == NULL)
		{
			current = current->next;
			continue;
		}
		if (token->type == redir_in || token->type == redir_out || token->type == redir_app || token->type == heredoc)
		{
			new_redir = ft_lstnew(token_text);
			if (token->type == redir_in || token->type == heredoc)
			{
				ft_lstadd_back(&(block->redir_in), new_redir);
				printf("Redirection in added: %s\n", token_text);
			}
			else
			{
				ft_lstadd_back(&(block->redir_out), new_redir);
				printf("Redirection out added: %s\n", token_text);
			}
		}
		else
			parse_command_option(token_text, block);
		current = current->next;
	}
}

void	print_cmd_block(t_cmd_block *cmd_block)
{
		t_list		*opt;
		t_list		*arg;
		t_list		*redir;

		if (cmd_block->option)
		{
			opt = cmd_block->option;
			while (opt)
			{
				printf("  Option: %s\n", (char *)opt->content);
				opt = opt->next;
			}
		}
		if (cmd_block->arg)
		{
			arg = cmd_block->arg;
			while (arg)
			{
				printf("  Argument: %s\n", (char *)arg->content);
				arg = arg->next;
			}
		}
		if (cmd_block->redir_in)
		{
			redir = cmd_block->redir_in;
			while (redir)
			{
				printf("Redirections in:\n");
				printf("  %s\n", ((t_token_lst *)redir->content)->text);
				redir = redir->next;
			}
		}
		if (cmd_block->redir_out)
		{
			redir = cmd_block->redir_out;
			while (redir)
			{
				printf("Redirections out:\n");
				printf("  %s\n", ((t_token_lst *)redir->content)->text);
				redir = redir->next;
			}
		}
}

void	expand_ast(t_ast_node *node)
{
	t_cmd_block	*cmd_block;

	if (node == NULL)
		return ;
	if (node->type == command)
	{
		node->cmd_block = init_cmd_block();
		fill_cmd_block(node->cmd_block, node->tokens);
	}
	expand_ast(node->left);
	expand_ast(node->right);
}

void	clear_cmd_block(t_cmd_block *block)
{
	ft_lstclear(&(block->option), free);
	ft_lstclear(&(block->arg), free);
	ft_lstclear(&(block->redir_in), free);
	ft_lstclear(&(block->redir_out), free);
	free(block->command);
	free(block);
}
