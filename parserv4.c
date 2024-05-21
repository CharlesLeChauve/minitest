#include "minishell.h"

int	ft_isshelloperator(char c)
{
	if (c == '|' || c == '&')
		return (1);
	return (0);
}

void	ft_add_char_to_buffer(char **buffer, char c, size_t *len)
{
	char	*new_buffer;

	new_buffer = malloc(*len + 2);
	if (!new_buffer)
	{
		ft_printf("Error: Allocation failed\n");
		exit(EXIT_FAILURE);
	}
	if (*buffer)
		ft_memcpy(new_buffer, *buffer, *len);
	new_buffer[*len] = c;
	(*len)++;
	new_buffer[*len] = '\0';
	free(*buffer);
	*buffer = new_buffer;
}

void	set_quotes_state(t_tkn_info *tkn_info)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') && tkn_info->state == reg)
	{
			if (*tkn_info->curr_char == '"')
				tkn_info->state = dquote;
			else
				tkn_info->state = quote;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
	{
		tkn_info->state = reg;
	}
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
	{
		tkn_info->state = reg;
	}
}

void	first_read_quotes(t_tkn_info *tkn_info, char **buffer, size_t *len)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') && tkn_info->state == reg)
	{
			if (*tkn_info->curr_char == '"')
				tkn_info->state = dquote;
			else
				tkn_info->state = quote;
			ft_add_char_to_buffer(buffer, *tkn_info->curr_char, len);
			tkn_info->curr_char++;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
	{
		ft_add_char_to_buffer(buffer, *tkn_info->curr_char, len);
		tkn_info->state = reg;
		tkn_info->curr_char++;
	}
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
	{
		ft_add_char_to_buffer(buffer, *tkn_info->curr_char, len);
		tkn_info->state = reg;
		tkn_info->curr_char++;
	}
}

void	space_quotes(t_tkn_info *tkn_info)
{
	if (tkn_info->state == reg)
		while (ft_isspace(*(tkn_info->curr_char)))
			tkn_info->curr_char++;
	set_quotes_state(tkn_info);
}

int	break_token(t_tkn_info *tkn_info,  char **buffer, size_t *len)
{
	first_read_quotes(tkn_info, buffer, len);
	if (tkn_info->state == reg && (ft_isshelloperator(*tkn_info->curr_char) || *tkn_info->curr_char == ' '))
		return (1);
	else
		return (0);
}

void	set_token_text(t_tkn_info *tkn_info, t_token_lst *token)
{
	size_t		len;
	char		*buffer;

	len = 0;
	buffer = NULL;
	while (*tkn_info->curr_char)
	{
		// set_quotes_state(tkn_info);
		if (tkn_info->curr_char != tkn_info->input && break_token(tkn_info, &buffer, &len))
			break ;
		ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len);
		tkn_info->curr_char++;
	}
	while (len > 0 && ft_isspace(buffer[len - 1]))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	token->text = buffer;
}

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
	if (*curr_char == '\0' || *curr_char == '\n')
	{
		ft_putstr_fd("tash : syntax error near unexpected token `newline'\n", 2);
		exit(0);
	}
	while (*curr_char && !ft_isshelloperator(*curr_char) && !ft_isspace(*curr_char))
	{
		ft_add_char_to_buffer(&buffer, *curr_char, &len);
		curr_char++;
	}
	token->text = buffer;
	return token;
}


t_token_lst	*cmd_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = command;
	// set_quotes_state(tkn_info);
	set_token_text(tkn_info, token);
	return (token);
}

t_token_lst	*next_token(t_tkn_info *tkn_info)
{
	space_quotes(tkn_info);
	if (!*tkn_info->curr_char)
		return (token_new(eol, NULL));
	if (*tkn_info->curr_char == '|' && tkn_info->state == reg)
	{
		tkn_info->curr_char++;
		if (*tkn_info->curr_char == '|')
		{
			tkn_info->curr_char++;
			return (token_new(or_op, ft_strdup("||")));
		}
		return (token_new(pipe_op, ft_strdup("|")));
	}
	else if (*tkn_info->curr_char == '&' && *(tkn_info->curr_char + 1) == '&' && tkn_info->state == reg)
	{
		tkn_info->curr_char += 2;
		return (token_new(and_op, ft_strdup("&&")));
	}
	// if ((*tkn_info->curr_char == '>' || *tkn_info->curr_char == '<') \
	// 	&& tkn_info->state == reg)
	// 	return (redir_token(tkn_info));
	return (cmd_token(tkn_info));
}

t_dlist	*tokenize(char *input)
{
	t_dlist		*last;
	t_tkn_info	tkn_info;

	if (!input || !*input)
		return (NULL);
	tkn_info.input = ft_strdup(input);
	tkn_info.curr_char = tkn_info.input;
	tkn_info.token_lst = NULL;
	tkn_info.state = reg;
	tkn_info.quote_level = 0;
	ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(next_token(&tkn_info)));
	last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
	while (last && ((t_token_lst *)(last->content))->type != eol)
	{
		ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(next_token(&tkn_info)));
		last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
	}
	return (tkn_info.token_lst);
}
