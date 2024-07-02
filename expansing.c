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

void	set_quotes_state_in_redir(char **curr_char, t_sm *state, int *quotectrl)
{
	char	*start;

	start = *curr_char;
	if ((**curr_char == '"' || **curr_char == '\'') && *state == reg)
	{
		*quotectrl = 1;
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
	if (start != *curr_char && (**curr_char == '"' || **curr_char == '\''))
		set_quotes_state_in_redir(curr_char, state, quotectrl);
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

void handle_quotes_and_operators(char **curr_char, t_sm *state, char **buffer, size_t *len, int *quote)
{
	set_quotes_state_in_redir(curr_char, state, quote);
	if (**curr_char == '>' || **curr_char == '<')
	{
		ft_add_char_to_buffer(buffer, **curr_char, len);
		(*curr_char)++;
	}
}

void skip_whitespace(char **curr_char, t_sm *state, char **buffer, size_t *len)
{
	while (**curr_char == ' ' || **curr_char == '\t')
	{
		if (*state != reg)
			ft_add_char_to_buffer(buffer, **curr_char, len);
		(*curr_char)++;
	}
}

int handle_syntax_error(char **buffer, int quote)
{
	if (quote)
		return (1);
	ft_putstr_fd("tash : syntax error near unexpected token `newline'\n", 2);
	free(*buffer);
	return 0;
}

int is_end_of_token(char c)
{
	return ft_isrediroperator(c) || ft_isshelloperator(c) || c == '\0';
}

char *redir_token(char **str, t_sm *state)
{
	char *buffer;
	size_t len;
	int quote;

	buffer = NULL;
	len = 0;
	quote = 0;
	handle_quotes_and_operators(str, state, &buffer, &len, &quote);
	handle_quotes_and_operators(str, state, &buffer, &len, &quote);
	skip_whitespace(str, state, &buffer, &len);
	if (is_end_of_token(**str) && !handle_syntax_error(&buffer, quote))
		return (NULL);
	else if (is_end_of_token(**str) && !handle_syntax_error(&buffer, quote))
		return (buffer);
	while (**str && !ft_isshelloperator(**str) && !ft_isrediroperator(**str))
	{
		set_quotes_state_in_redir(str, state, &quote);
		if (*state == reg && ft_isspace(**str))
			break;
		ft_add_char_to_buffer(&buffer, **str, &len);
		(*str)++;
	}
	return buffer;
}


char *handle_special_chars(char **str, t_shell *shell)
{
	if (**str == '?')
	{
		(*str)++;
		return ft_itoa(shell->last_ret);
	}
	else if (**str == '$')
	{
		(*str)++;
		return ft_itoa(getpid());
	}
	return NULL;
}

char *get_env_value(char **str, t_shell *shell)
{
	char *temp;
	char *env_var_value;
	char *value_start;
	size_t j;

	j = 0;
	while ((*str)[j] && !ft_isspace((*str)[j]) && (*str)[j] != '$' && (*str)[j] != '\'' && (*str)[j] != '"' && (*str)[j] != '*')
		j++;
	temp = ft_substr(*str, 0, j);
	*str += j;
	env_var_value = get_env_var(shell->env, temp);
	free(temp);
	if (env_var_value)
	{
		value_start = ft_strchr(env_var_value, '=');
		if (value_start)
			return ft_strdup(value_start + 1);
	}
	return NULL;
}

char *extrapolate(char **str, t_shell *shell)
{
	char *result;

	(*str)++;
	if (!(**str) || (!ft_isalnum(**str) && (**str != '$' && **str != '?' && **str != '_')))
		return ft_strdup("$");
	result = handle_special_chars(str, shell);
	if (result)
		return result;
	return get_env_value(str, shell);
}

char *extract_command(char **ptr, t_shell *shell, int *is_a_redir)
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
		if (**ptr == '$' && state != quote)
		{
			ft_strappend(&buffer, extrapolate(ptr, shell), &len);
			continue ;
		}
		else if (same_quote(&state, **ptr))
		{
			if (!buffer)
				buffer = ft_strdup("");
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
			*is_a_redir = 1;
			return (buffer);
		}
		ft_add_char_to_buffer(&buffer, **ptr, &len);
		(*ptr)++;
	}
	return (buffer);
}


int	expand_redir(char **str)
{
	char	*suffix;
	char	*prefix;
	int     index;

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

int process_sub_token(char *sub_token, t_cmd_block *block, int is_a_redir)
{
	t_list	*new_arg;
	t_token_type	type;
	char		*str;

	if (is_a_redir && (sub_token[0] == '>' || sub_token[0] == '<'))
	{
		if (sub_token[1] && ((sub_token[1] == '<' && sub_token[0] == '>')\
			|| (sub_token[1] == '>' && sub_token[0] == '<')))
		{
			fprintf(stderr, "tash: Wrong redir operator.\n");
			return (1);
		}
		type = redir_type(sub_token);
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
	}
	else
	{
		new_arg = ft_lstnew(sub_token);
		ft_lstadd_back(&(block->arg), new_arg);
	}
	return (0);
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

// void	print_cmd_block(t_cmd_block *cmd_block)
// {
// 	t_list	*arg;
// 	t_list	*redir;

// 	if (cmd_block->arg)
// 	{
// 		arg = cmd_block->arg;
// 		while (arg)
// 		{
// 			printf("  Argument: %s\n", (char *)arg->content);
// 			arg = arg->next;
// 		}
// 	}
// 	if (cmd_block->redirs)
// 	{
// 		redir = cmd_block->redirs;
// 		while (redir)
// 		{
// 			printf("Redirections:\n");
// 			printf("  %s\n", ((t_token_lst *)redir->content)->text);
// 			redir = redir->next;
// 		}
// 	}
// }

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
