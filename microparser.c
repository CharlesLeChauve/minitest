#include "minishell.h"

typedef enum
{
    e_alpha, e_num, e_redir, e_pipe_op, e_and_op, e_or_op, e_semicolon,\
    e_dash, e_dot, e_backslash, e_dollar, e_slash, e_wildcard, e_unknown\
    e_quote, e_dquotes
}   t_symbol;

int accept(t_symbol expected, t_symbol *current_symbol, char **input)
{
    if (*current_symbol == expected)
    {
        next_symbol(symbol, input);
        return (1);
    }
    return (0);
}

void    next_symbol(t_symbol *symbol, char *input)
{
    *input++;
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
        *symbol = e_
    else if (**input == '/')
        *symbol = e_slash;
    else if (**input == '\\')
        *symbol = e_backslash;
    else if (**input == '$')
        *symbol = e_dollar;
    else if (**input == '?' || **input == '*')
        *symbol = e_wildcard;
}

void    command(t_symbol *current_symbol, char **input)
{

}

t_token_lst *token(t_symbol *sym, t_token_lst **token_lst, char **input)
{
    t_token *token;

    token = (t_token *)malloc(sizeof)
    if (accept(e_alpha))
        command(sym, input);
}

t_token_lst *parse_input_char(char *input)
{
    t_symbol    symbol;
    t_token_lst *token_lst;
    char *      input_cpy;

    input_cpy = ft_strdup(input);
    symbol = e_unknown;
    next_symbol(&symbol, &input);
    token(&symbol, &token_lst, &input);
}