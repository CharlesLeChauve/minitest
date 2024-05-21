#include "minishell.h"

t_cmd_block	*init_cmd_block(void)
{
	t_cmd_block	*block;

	block = malloc(sizeof(t_cmd_block));
	if (!block)
		return (NULL);
	block->exec_tab = NULL;
	block->commande = NULL;
	block->option = NULL;
	block->arg = NULL;
	block->redir_in = NULL;
	block->redir_out = NULL;
	return (block);
}

void	extract_command(char **ptr, t_cmd_block *block)
{
	char	*start = *ptr;

	while (**ptr && !ft_isspace(**ptr))
		(*ptr)++;
	if (*ptr > start)
		block->commande = ft_strndup(start, *ptr - start);
}

void	set_quotes_state_in_cmd_block(char **curr_char, t_sm *state)
{
	if ((**curr_char == '"' || **curr_char == '\'') && *state == reg)
	{
		if (**curr_char == '"')
			*state = dquote;
		else
			*state = quote;
		(*curr_char)++;
	}
	else if (**curr_char == '"' && *state == dquote)
	{
		*state = reg;
		(*curr_char)++;
	}
	else if (**curr_char == '\'' && *state == quote)
	{
		*state = reg;
		(*curr_char)++;
	}
}

int	break_cmd_sub_token(char **curr_char, t_sm *state)
{
	set_quotes_state_in_cmd_block(curr_char, state);
	if (state == reg && (ft_isshelloperator(**curr_char)) || **curr_char == ' ' || **curr_char == '>' || **curr_char == '<')
		return (1);
	else
		return (0);
}

char	*extract_sub_token(char **ptr)
{
	t_sm	state;
	char	*buffer = NULL;
	size_t len = 0;

	state = reg;
	while (*ptr)
	{
		if (break_cmd_sub_token(ptr, &state))
			break ;
		ft_add_char_to_buffer(&buffer, **ptr, &len);
		(*ptr)++;
	}
	return (buffer);
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

	extract_command(&ptr, block);
	while (*ptr)
	{
		while (ft_isspace(*ptr))
			ptr++;
		if (*ptr)
		{
			sub_token = extract_sub_token(&ptr);
			process_sub_token(sub_token, block);
		}
	}
}


// void	parse_command_option(char *token, t_cmd_block *block)
// {
// 	char	*ptr;
// 	char	*start;
// 	char	*sub_token;
// 	t_list	*new_opt;
// 	t_list	*new_arg;
// 	int		in_quotes = 0;
// 	char	quote_char = '\0';

// 	ptr = token;
// 	while (*ptr && !ft_isspace(*ptr))
// 		ptr++;
// 	if (ptr > token)
// 		block->commande = ft_strndup(token, ptr - token);

// 	while (*ptr)
// 	{
// 		while (ft_isspace(*ptr))
// 			ptr++;
// 		start = ptr;

// 		while (*ptr && (in_quotes || !ft_isspace(*ptr)))
// 		{
// 			if (*ptr == '"' || *ptr == '\'')
// 			{
// 				if (in_quotes && *ptr == quote_char)
// 					in_quotes = 0;
// 				else if (!in_quotes)
// 				{
// 					in_quotes = 1;
// 					quote_char = *ptr;
// 				}
// 			}
// 			ptr++;
// 		}
// 		if (ptr > start)
// 		{
// 			sub_token = ft_strndup(start, ptr - start);
// 			if (sub_token[0] == '-')
// 			{
// 				new_opt = ft_lstnew(sub_token);
// 				ft_lstadd_back(&(block->option), new_opt);
// 			}
// 			else if (sub_token[0] == '>' || sub_token[0] == '<')
// 			{
// 				new_arg = ft_lstnew(redir_token(sub_token));
// 				if (sub_token[0] == '>')
// 					ft_lstadd_back(&(block->redir_out), new_arg);
// 				if (sub_token[0] == '<')
// 					ft_lstadd_back(&(block->redir_in), new_arg);
// 			}
// 			else
// 			{
// 				new_arg = ft_lstnew(sub_token);
// 				ft_lstadd_back(&(block->arg), new_arg);
// 			}
// 		}
// 	}
// }


void	fill_cmd_block(t_cmd_block *block, t_dlist *tokens)
{
	t_dlist		*current = tokens;
	t_list		*new_redir;
	t_list		*redir;
	t_token_lst	*token;
	t_list		*opt;
	t_list		*arg;
	char		*token_text;

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
	//Le commande block cree a l'air nickel mais il n'est jamais affecte a rien, on le erd apres cette fonction
	//est-ce qu'on ajoute un pointeur a la structure ast_node ?
	//
	if (node->type == command)
	{
		//cmd_block = init_cmd_block();
		node->cmd_block = init_cmd_block();
		fill_cmd_block(node->cmd_block, node->tokens);
		// node->cmd_block = cmd_block;
		// print_cmd_block(node->cmd_block);
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
	free(block->commande);
	free(block);
}
