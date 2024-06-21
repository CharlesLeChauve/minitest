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

void	first_read_quotes(t_tkn_info *tkn_info)
{
	if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'') && tkn_info->state == reg)
	{
		if (*tkn_info->curr_char == '"')
			tkn_info->state = dquote;
		else
			tkn_info->state = quote;
	}
	else if (*tkn_info->curr_char == '"' && tkn_info->state == dquote)
		tkn_info->state = reg;
	else if (*tkn_info->curr_char == '\'' && tkn_info->state == quote)
		tkn_info->state = reg;
}

void	space_quotes(t_tkn_info *tkn_info)
{
	if (tkn_info->state == reg)
		while (ft_isspace(*(tkn_info->curr_char)))
			tkn_info->curr_char++;
	set_quotes_state(tkn_info);
}

int	break_token(t_tkn_info *tkn_info)
{
	first_read_quotes(tkn_info);
	if ((tkn_info->state == reg && ft_isshelloperator(*tkn_info->curr_char)))
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
		if (tkn_info->curr_char != tkn_info->input && break_token(tkn_info))
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

t_token_lst	*cmd_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = command;
	set_token_text(tkn_info, token);
	return (token);
}

t_token_lst	*subshell_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;
	size_t		len;
	char		*buffer;
	int			par_lvl;

	par_lvl = 0;
	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = subshell;
	len = 0;
	buffer = NULL;
	while (*tkn_info->curr_char)
	{
		if (*tkn_info->curr_char == '(')
			par_lvl++;
		if (*tkn_info->curr_char == ')' && par_lvl)
			par_lvl--;
		else if (*tkn_info->curr_char == ')')
		{
			tkn_info->curr_char++;
			break ;
		}
		ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len);
		tkn_info->curr_char++;
	}
	if (buffer)
		buffer[ft_strlen(buffer)] = '\0';
	token->text = buffer;
	return (token);
}

t_token_lst	*next_token(t_tkn_info *tkn_info)
{
	space_quotes(tkn_info);
	if (!*tkn_info->curr_char)
		return (token_new(eol, NULL));
	if (*tkn_info->curr_char == '(')
	{
		tkn_info->curr_char++;
		return (subshell_token(tkn_info));
	}
	if (*tkn_info->curr_char == '|' && tkn_info->state == reg)
	{
		tkn_info->curr_char++;
		if (*tkn_info->curr_char == '|')
		{
			tkn_info->curr_char++;
			if (*tkn_info->curr_char == '|')
				return (NULL);
			return (token_new(or_op, "||"));
		}
		else if (*tkn_info->curr_char == '&')
			return (NULL);
		return (token_new(pipe_op, "|"));
	}
	else if (*tkn_info->curr_char == '&' && tkn_info->state == reg)
	{
		tkn_info->curr_char++;
		if (*tkn_info->curr_char == '&')
		{
			tkn_info->curr_char++;
			if (*tkn_info->curr_char == '&')
				return (NULL);
			return (token_new(and_op, "&&"));
		}
		else if (*tkn_info->curr_char == '|')
			return (NULL);
		return (NULL);
	}
	return (cmd_token(tkn_info));
}

t_dlist *tokenize(char *input)
{
    t_dlist *last;
    t_tkn_info tkn_info;
    t_token_lst *new_token;

    if (!input || !*input)
        return (NULL);
    tkn_info.input = ft_strdup(input);
    if (!tkn_info.input)
    {
        ft_printf("Error: Memory allocation failed\n");
        return (NULL);
    }
    tkn_info.curr_char = tkn_info.input;
    tkn_info.token_lst = NULL;
    tkn_info.state = reg;

    new_token = next_token(&tkn_info);
    if (!new_token)
    {
        ft_printf("Syntax error: invalid token\n");
        free(tkn_info.input);
        return (NULL);
    }
    ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(new_token));
    last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);

    while (last && ((t_token_lst *)(last->content))->type != eol)
    {
        new_token = next_token(&tkn_info);
        if (!new_token)
        {
            ft_putstr_fd("Syntax error: invalid token.\n", 2);
            free(tkn_info.input);
            ft_dlstclear(&tkn_info.token_lst, del_tkn_node);
            return (NULL);
        }
        ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(new_token));
        last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
    }
    free(tkn_info.input);
    return (tkn_info.token_lst);
}

