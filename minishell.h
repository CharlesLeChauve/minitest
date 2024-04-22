#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"



typedef enum e_token_type
{
	command, redir_in, redir_out, redir_app, heredoc, pipe_op, fd, file_path, and_op, or_op, eol
}	t_token_type;

typedef enum e_redir_type
{
	in, out, append, heredoc
}	t_redir_type;

typedef enum e_state_machine
{
	reg, quote, dquote
}	t_sm;

typedef enum {
	COMMAND,
	PIPE,
	AND,
	OR
} NodeType;

typedef struct s_token_lst
{
	t_token_type type;
	union
	{
		char	*text;
		int		fd;
	};
	struct s_token_lst	*next;
}	t_token_lst;

typedef	struct	s_tkn_info
{
	t_token_lst	*token_lst;
	t_sm		state;
	char		*input;
	char		*curr_char;
}	t_tkn_info;

// Mise à jour de la structure Node pour inclure des informations sur les redirections
typedef struct s_ast_node {
	t_token_type    type;
	t_token_lst     *tokens;
	char			*value;
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;

// utils
void		swap_char(char *c1, char *c2);
void		revstr(char *str);

// lexing
void		lexer(char	*input, t_token_lst **tokens);
t_token_lst	*token_new(t_token_type type, void *content);
t_token_lst 	*token_last(t_token_lst *token_lst);
void		tokenaddback(t_token_lst **lst, t_token_lst *node);
void		treat_token(char *token, t_token_lst **token_lst);
void		print_token_type(t_token_lst *token);

// parsing
t_token_lst	*tokenize(char *input);
void		parser(t_token_lst *tokens);
int		    ft_isvalidchar(char c);
void		parse_input(char *input, t_token_lst **token_lst);

// signal
void		setup_signal_handlers(void);
void		handle_sigint(int sig);

// execution (faut la faire mdr)

// memory
void		free_tokens(t_token_lst	*token);
t_ast_node  *parse_tokens(t_token_lst *tokens);
void        print_tree(t_ast_node *root); 

#endif
