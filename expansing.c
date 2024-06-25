#include "minishell.h"

int	ft_isrediroperator(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

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

int	same_quote(t_sm *state, char c)
{
	if (*state == dquote && c == '"')
		return (1);
	else if (*state == quote && c == '\'')
		return (1);
	return (0);
}

t_token_type	redir_type(char *curr_char)
{
	if (*curr_char == '>')
	{
		curr_char++;
		if (*curr_char == '>')
		{
			curr_char++;
			return redir_app;
		}
		else
			return redir_out;
	}
	else if (*curr_char == '<')
	{
		curr_char++;
		if (*curr_char == '<')
		{
			curr_char++;
			return heredoc;
		}
		else
			return redir_in;
	}
	return 11;
}

char	*redir_token(char **str, t_sm *state)
{
	char		**curr_char;
	char		*buffer;
	size_t		len;

	curr_char = str;
	buffer = NULL;
	len = 0;
	if (*state == dquote || *state == quote)
		return (NULL);
	set_quotes_state_in_cmd_block(curr_char, state);
	if (**curr_char == '>' || **curr_char == '<')
	{
		ft_add_char_to_buffer(&buffer, **curr_char, &len);
		(*curr_char)++;
	}
	set_quotes_state_in_cmd_block(curr_char, state);
	if (**curr_char == '>' || **curr_char == '<')
	{
		ft_add_char_to_buffer(&buffer, **curr_char, &len);
		(*curr_char)++;
	}
	while (**curr_char == ' ' || **curr_char == '\t')
		(*curr_char)++;
	if ( **curr_char == '\0')
	{
		free(buffer);
		return (NULL);
	}
	if (ft_isrediroperator(**curr_char) || ft_isshelloperator(**curr_char))
	{
		ft_putstr_fd("tash : syntax error near unexpected token `newline'\n", 2);
		// shell->last_ret = 2;
		free(buffer);
		return (NULL);
	}
	while (**curr_char && !ft_isshelloperator(**curr_char) && !ft_isrediroperator(**curr_char) && !ft_isspace(**curr_char))
	{
		set_quotes_state_in_cmd_block(curr_char, state);
		ft_add_char_to_buffer(&buffer, **curr_char, &len);
		(*curr_char)++;
	}
	return (buffer);
}

char	*extrapolate_2(char **str, t_shell *shell, t_sm *state)
{
	char	*result;
	char	*temp;
	char	*env_var_value;
	char	*value_start;
	size_t	j;

	result = NULL;
	temp = NULL;
	env_var_value = NULL;
	value_start = NULL;
	j = 0;
	if (*state == quote)
		return (NULL);
	if (**str == '$')
	{
		
		(*str)++;
		if (!(**str) || (!ft_isalnum(**str) && (**str != '$' && **str != '?')))
			return (ft_strdup("$"));
		if (**str == '?')
		{
			(*str)++;
			result = ft_itoa(shell->last_ret);
			return (result);
		}
		  else if (**str == '$')
        {
            (*str)++;
            result = ft_itoa(getpid());
            return (result);
        }
		while ((*str)[j] && !ft_isspace((*str)[j]) && (*str)[j] != '$'\
				&& (*str)[j] != '\'' && (*str)[j] != '"' && (*str)[j] != '*')
			j++;
		temp = ft_substr(*str, 0, j);
		*str += j;
		env_var_value = get_env_var(shell->env, temp);
		free(temp);
		if (env_var_value)
		{
			value_start = ft_strchr(env_var_value, '=');
			if (value_start)
				result = ft_strdup(value_start + 1);
		}
		else
			result = (ft_strdup(""));
	}
	return (result);
}

char *extract_command(char **ptr, t_shell *shell, int *is_a_redir)
{
	size_t	len;
	char	*buffer;
	char	*ext;
	t_sm	state;

	len = 0;
	buffer = ft_strdup("");
	state = reg;
	while (**ptr)
	{
		set_quotes_state_in_cmd_block(ptr, &state);
		ext = NULL;
		ext = extrapolate_2(ptr, shell, &state);
		if (ext)
		{
			ft_strappend(&buffer, ext, &len);
			free(ext);
			continue ;
		}
		else if (same_quote(&state, **ptr))
		{
			//(*ptr)++;
			continue ;
		}
		else if (!**ptr || (state == reg && ft_isspace(**ptr)))
			break ;
		else if (state == reg && (**ptr == '>' || **ptr == '<'))
		{
			if (buffer)
			{
				free(buffer);
				buffer = NULL;
			}
			buffer = redir_token(ptr, &state);
			if (!buffer)
			{
				ft_putstr_fd("tash: syntax error near unexpected token 'newline'\n", 2);
			}
			*is_a_redir = 1; // j'arrive pas a envoyer un message d'erreur type command not found (">out" ne renvoie pas command not found)
			return (buffer);
		}
		ft_add_char_to_buffer(&buffer, **ptr, &len);
		(*ptr)++;
	}
	return (buffer);
}

void process_sub_token(char *sub_token, t_cmd_block *block, int is_a_redir)
{
	t_list	*new_arg;
	t_token_type	type;
	char		*str;

	if (is_a_redir && (sub_token[0] == '>' || sub_token[0] == '<'))
	{
		if ((sub_token[1] == '<' && sub_token[0] == '>') \
		|| (sub_token[1] == '>' && sub_token[0] == '<') \
		|| (sub_token[2] == '>') || (sub_token[2] == '<'))
		{
			fprintf(stderr, "tash: Wrong redir operator.\n");
			return ;
		}
		type = redir_type(sub_token);
		if (type == heredoc || type == redir_app)
			str = ft_strdup(sub_token + 2);
		else if (type == redir_out || type == redir_in)
			str = ft_strdup(sub_token + 1);
		free(sub_token);
		new_arg = ft_lstnew(token_new(type, str));
		free(str);
		if (new_arg)
			ft_lstadd_back(&(block->redirs), new_arg);
		else
		{
		    free(sub_token);
		    sub_token = NULL;
		}
	}
	else
	{
		new_arg = ft_lstnew(sub_token);
		ft_lstadd_back(&(block->arg), new_arg);
	}
}

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
				clear_cmd_block(block);
				//clean_shell_instance(shell);
				return (1);
			}
			process_sub_token(sub_token, block, is_a_redir);
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

void	print_cmd_block(t_cmd_block *cmd_block)
{
	t_list	*arg;
	t_list	*redir;

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
			return (1);
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
	// creer une fonction qui parcourt mes listes et free lst->content->text;
	ft_lstiter(block->redirs, free_token_lst_content);
	ft_lstclear(&(block->arg), free);
	ft_lstclear(&(block->redirs), free);
	if (block->exec_tab)
		ft_free_tab(block->exec_tab);
	free(block->command);
	free(block);
}
