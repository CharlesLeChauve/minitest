#include "minishell.h"

typedef enum e_symbol
{
	alpha, num, redir, pipe_op, and_op, or_op, semicolon,\
	dash, dot, backslash, dollar, slash, wildcard, unknown,\
	quote, dquotes
}   t_symbol;

void    next_symbol(t_symbol *symbol, char *input)
{
	if (ft_isalpha(**input))
		*symbol = e_alpha;
	else if (ft_isalnum(**input))
		*symbol = e_num;
	else if (**input == '<' || **input == '>')
		*symbol = e_redir;
	else if (**input == '&' && **input + 1 == '&')
	{
		*input++;
		*symbol = e_and_op;
	}
	else if (**input == '|' && **input + 1 == '|')
	{
		*input++;
		*symbol = e_or_op;
	}
	else if (**input == '|')
		*symbol = e_pipe_op;
	else if (**input == ';')
		*symbol = e_semicolon;
	else if (**input == '-')
		*symbol = e_dash;
	else if (**input == '.')
		*symbol = e_dot;
	else if (**input == '/')
		*symbol = e_slash;
	else if (**input == '\\')
		*symbol = e_backslash;
	else if (**input == '$')
		*symbol = e_dollar;
	else if (**input == '?' || **input == '*')
		*symbol = e_wildcard;
	*input++;
}

int accept(t_symbol expected, t_symbol *current_symbol, char **input)
{
	if (*current_symbol == expected)
	{
		next_symbol(symbol, input);
		return (1);
	}
	return (0);
}

void    command(t_symbol *current_symbol, char **input)
{
	while ()
}

t_token_lst *token(t_symbol *sym, t_token_lst **token_lst, char **input)
{
	t_token_lst *token;

	token = (t_token_lst *)malloc(sizeof)
	if (accept(e_alpha))
		command(sym, input);
	
}

t_token_lst *parse_input_char(char *input)
{
	t_symbol    symbol;
	t_token_lst *token_lst;
	char        *input_cpy;

	input_cpy = ft_strdup(input);
	symbol = e_unknown;
	next_symbol(&symbol, &input);
	token(&symbol, &token_lst, &input);
}