#include "minishell.h"

t_token_lst *redir_token(char *str)
{
	char	*curr_char = str;
	t_token_lst	*token;
	char		*buffer;
	size_t		len;

	buffer = NULL;
	len = 0;
	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!token)
	{
		perror("Allocation Failed");
		exit(EXIT_FAILURE);
	}
	if (*curr_char == '>')
	{
		curr_char++;
		if (*curr_char == '>')
		{
			curr_char++;
			token->type = redir_app;
		}
		else
			token->type = redir_out;
	}
	else if (*curr_char == '<')
	{
		curr_char++;
		if (*curr_char == '<')
		{
			curr_char++;
			token->type = heredoc;
		}
		else
			token->type = redir_in;
	}
	while (*curr_char == ' ')
		curr_char++;
	if (ft_isshelloperator(*curr_char) || *curr_char == '\0')
	{
		ft_putstr_fd("tash : syntax error near unexpected token `newline'\n", 2);
		return (NULL);
	}
	while (*curr_char && !ft_isshelloperator(*curr_char) && !ft_isspace(*curr_char))
	{
		ft_add_char_to_buffer(&buffer, *curr_char, &len);
		curr_char++;
	}
	token->text = buffer;
	return token;
}

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
	block->redirs = NULL;
	return (block);
}

void	set_quotes_state_in_cmd_block(char **curr_char, t_sm *state)
{
	if ((**curr_char == '"' || **curr_char == '\'') && *state == reg)
	{
		if (**curr_char == '"')
		{
			*state = dquote;
			// printf("a\n");
		}
		else
			*state = quote;
		(*curr_char)++;
	}
	else if (**curr_char == '"' && *state == dquote)
	{
		// printf("b\n");
		*state = reg;
		(*curr_char)++;
	}
	else if (**curr_char == '\'' && *state == quote)
	{
		*state = reg;
		(*curr_char)++;
	}
}

void	extract_command(char **ptr, t_cmd_block *block)
{
	size_t	len;
	char	*buffer;
	t_sm	state;

	len = 0;
	buffer = NULL;
	state = reg;
	while (**ptr)
	{
		set_quotes_state_in_cmd_block(ptr, &state);
		if (**ptr == '"' || **ptr == '\'')
			continue ;
		if ((state == reg && (**ptr == '>' || **ptr == '<')) || (ft_isspace(**ptr) && state == reg))
			break ;
		ft_add_char_to_buffer(&buffer, **ptr, &len);
		(*ptr)++;
	}
	while (len > 0 && ft_isspace(buffer[len - 1]))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	block->command = buffer;
	// printf("commande = %s\n", block->command);
}

char	*extract_sub_token(char **ptr)
{
	char	*buffer;
	t_sm	state;
	size_t	len;

	buffer = NULL;
	len = 0;
	state = reg;
	if (**ptr == '<' || **ptr == '>')
	{
		while (**ptr == '>' || **ptr == '<')
		{
			ft_add_char_to_buffer(&buffer, **ptr, &len);
			(*ptr)++;
		}
		while (**ptr == ' ')
			(*ptr)++;
	}
	while (**ptr && (state != reg || !ft_isspace(**ptr)))
	{
		set_quotes_state_in_cmd_block(ptr, &state);
		if (state == reg && (ft_isspace(**ptr) || ft_isshelloperator(**ptr)))
			break;
		ft_add_char_to_buffer(&buffer, **ptr, &len);
		(*ptr)++;
	}
	while (len > 0 && ft_isspace(buffer[len - 1]))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	// printf("%s\n", buffer);
	return (buffer);
}

void	process_sub_token(char *sub_token, t_cmd_block *block, char **env)
{
	t_list	*new_arg;
	char	*env_var_value;

	if (sub_token[0] == '$')
	{
		// if (sub_token[1] == '?' && sub_token[2] == '\0')
		// {
		// 	char last_ret_str[12]; 12 parce que l'int min c'est 11 pour la longueur de int min + \0
		// 	sprintf(last_ret_str, "%d", shell->last_ret);
		// 	free(sub_token);
		// 	sub_token = ft_strdup(last_ret_str);
		// }
		// else
		if (!sub_token[1])
		{
			new_arg = ft_lstnew(sub_token);
			ft_lstadd_back(&(block->arg), new_arg);
			printf("sub token = %s\n", sub_token);
			return ;
		}
		{
			env_var_value = get_env_var(env, sub_token + 1);
			if (env_var_value)
			{
				free(sub_token);
				sub_token = ft_strdup(env_var_value);
			}
			else
			{
				fprintf(stderr, "tash: %s: Undefined variable\n", sub_token + 1);
				free(sub_token);
				return ;
			}
		}
	}
	if (sub_token[0] == '>' || sub_token[0] == '<')
	{
		if ((sub_token[1] == '<' && sub_token[0] == '>') || (sub_token[1] == '>' && sub_token[0] == '<') || (sub_token[2] == '>') || (sub_token[2] == '<'))
		{
			fprintf(stderr, "tash: Wrong redir operator.\n");
			return ;
		}
		new_arg = ft_lstnew(redir_token(sub_token));
		if (new_arg)
			ft_lstadd_back(&(block->redirs), new_arg);
		// else
		// {
		// 	free(sub_token);
		// 	sub_token = NULL;
		// } ca free trop tot je crois tonton
	}
	else
	{
		new_arg = ft_lstnew(sub_token);
		ft_lstadd_back(&(block->arg), new_arg);
	}
}

void	parse_command_option(char *token, t_cmd_block *block, char **env)
{
	char	*ptr = token;
	char	*sub_token;
	int		command_found;

	command_found = 0;
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
				process_sub_token(sub_token, block, env);
			}
		}
	}
}

void	fill_cmd_block(t_cmd_block *block, t_dlist *tokens, char **env)
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
			continue;
		}
		parse_command_option(token_text, block, env);
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
		if (cmd_block->redirs)
		{
			redir = cmd_block->redirs;
			while (redir)
			{
				printf("Redirections:\n");
				printf("  %s\n", ((t_token_lst *)redir->content)->text);
				redir = redir->next;
			}
		}
}

void	expand_ast(t_ast_node *node, t_shell *shl)
{
	t_cmd_block	*cmd_block;

	if (node == NULL)
		return ;
	if (node->type == command)
	{
		cmd_block = init_cmd_block();
		fill_cmd_block(cmd_block, node->tokens, shl->env);
		create_exec_tab(cmd_block);
		if (get_heredocs(cmd_block))
			return ;
		node->cmd_block = cmd_block;
	}
	expand_ast(node->left, shl);
	expand_ast(node->right, shl);
}

void	clear_cmd_block(t_cmd_block *block)
{
	ft_lstclear(&(block->option), free);
	ft_lstclear(&(block->arg), free);
	ft_lstclear(&(block->redirs), free);
	free(block->command);
	free(block);
}
