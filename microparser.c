#include "minishell.h"

typedef enum e_symbol
{
	alpha, num, redir, pipe_op, and_op, or_op, semicolon,\
	dash, dot, backslash, dollar, slash, wildcard, unknown,\
	quote, dquotes, eol
}   t_symbol;

typedef	struct	s_tkn_info
{
	t_symbol	symbol;
	t_token_lst	*token_lst;
	char		*input;
	char		*curr_char;
}	t_tkn_info;

void    next_symbol(t_tkn_info *tkn_info)
{
	if (ft_isalpha(*(tkn_info->input)))
		tkn_info->symbol = alpha;
	else if (ft_isalnum(*(tkn_info->input)))
		tkn_info->symbol = num;
	else if (*(tkn_info->input) == '<' || *(tkn_info->input) == '>')
		tkn_info->symbol = redir;
	else if (*(tkn_info->input) == '&' && *(tkn_info->input + 1) == '&')
	{
		tkn_info->input++;
		tkn_info->symbol = and_op;
	}
	else if (*(tkn_info->input) == '|' && *(tkn_info->input + 1) == '|')
	{
		tkn_info->input++;
		tkn_info->symbol = or_op;
	}
	else if (*(tkn_info->input) == '|')
		tkn_info->symbol = pipe_op;
	else if (*(tkn_info->input) == ';')
		tkn_info->symbol = semicolon;
	else if (*(tkn_info->input) == '-')
		tkn_info->symbol = dash;
	else if (*(tkn_info->input) == '.')
		tkn_info->symbol = dot;
	else if (*(tkn_info->input) == '/')
		tkn_info->symbol = slash;
	else if (*(tkn_info->input) == '\\')
		tkn_info->symbol = backslash;
	else if (*(tkn_info->input) == '$')
		tkn_info->symbol = dollar;
	else if (*(tkn_info->input) == '?' || *(tkn_info->input) == '*')
		tkn_info->symbol = wildcard;
	else if (*(tkn_info->input) == '\0' || *(tkn_info->input) == '\n')
		tkn_info->symbol = eol;
	tkn_info->input++;
}

void    ft_add_char_to_buffer(char **buffer, char c, size_t *len)
{
    char    *new_buffer;

    new_buffer = malloc(*len + 2);
    if (!new_buffer)
    {
        ft_printf("Error: Allocation failed\n");
        exit(EXIT_FAILURE);
    }
    ft_memcpy(new_buffer, *buffer, *len);
    new_buffer[*len] = c;
    (*len)++;
    new_buffer[*len] = '\0';
    free(*buffer);
    *buffer = new_buffer;
}

int accept(t_symbol expected, t_tkn_info *tkn_info)
{
	if (tkn_info->symbol == expected)
	{
		next_symbol(tkn_info);
		return (1);
	}
	return (0);
}

t_token_lst	*next_token(t_tkn_info *tkn_info)
{
	char		*text;
	t_token_lst	*new_token;
	t_token_lst	*last;

	last = token_last(tkn_info->token_lst);
	while (ft_isspace(*(tkn_info->curr_char)))
		tkn_info->curr_char++;
	if (*tkn_info->curr_char == '\0' || tkn_info->curr_char == '\n')
		return (token_new(eol, NULL));
	if (ft_isalpha(*tkn_info->curr_char) || *tkn_info->curr_char == '/' || *tkn_info->curr_char == '.')
	{
		while (ft_isalnum(*tkn_info->curr_char) || tkn_info->curr_char == '_'\
				|| tkn_info->curr_char == '-' || *tkn_info->curr_char == '/')
			ft_add_char_to_buffer(&text, *tkn_info->curr_char, ft_strlen(text));
		if (!last || last->type != command)
			return (token_new(command, text));
		if (last->type == command || last->type == argument || last->type == option)
			return (token_new(argument, text));
		if (last->type == redirection)
			return (token_new(file_path, text));
		if ()
	}
}

void	tokenize(t_tkn_info *tkn_info)
{
	while (token_last(tkn_info->token_lst) != eol)
	{
		tokenaddback(&tkn_info->token_lst, next_token(tkn_info));
	}

	
}

t_token_lst *parse_input_char(char *input)
{
	t_tkn_info	tkn_info;

	tkn_info.input = ft_strdup(input);
	tkn_info.curr_char = input;
	tkn_info.symbol = unknown;
	tkn_info.token_lst = NULL;
	tokenize(&tkn_info);
}