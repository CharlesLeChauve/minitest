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

typedef enum
{
	command, argument, option, redirection, pipe_op, fd, file_path, and_op, or_op, eol
}	t_token_type;

typedef enum {
    COMMAND,
    PIPE,
    AND,
    OR
} NodeType;

// Définition des types de redirection
typedef enum {
    NO_REDIRECTION,
    INPUT_REDIRECTION,
    OUTPUT_REDIRECTION,
    APPEND_REDIRECTION
} RedirectionType;

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

// Mise à jour de la structure Node pour inclure des informations sur les redirections
typedef struct s_ast_node {
    t_token_type type;
	t_token_lst		*command_tokens;
    char *value;
    RedirectionType redirection_type; // Type de redirection associée à la commande
    char *redirection_file; // Nom du fichier de redirection
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
void		parser(t_token_lst *tokens);
int		ft_isvalidchar(char c);
void		parse_input(char *input, t_token_lst **token_lst);

// signal
void		setup_signal_handlers(void);
void		handle_sigint(int sig);

// execution (faut la faire mdr)

// memory
void		free_tokens(t_token_lst	*token);
t_ast_node *parse_tokens(t_token_lst *tokens);
void print_tree(t_ast_node *root);

#endif
