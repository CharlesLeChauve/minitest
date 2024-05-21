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
#include "env.h"



typedef enum e_token_type
{
	command, redir_in, redir_out, redir_app, heredoc, pipe_op, fd, file_path, and_op, or_op, eol
}	t_token_type;

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
		int		priority;
	};
}	t_token_lst;

typedef	struct	s_tkn_info
{
	t_dlist		*token_lst;
	t_sm		state;
	int			quote_level;
	char		*input;
	char		*curr_char;
	char		first_quote;
}	t_tkn_info;

typedef struct s_cmd_block
{
	char	**exec_tab;
	char	*commande;
	t_list	*option;
	t_list	*arg;
	t_list	*redir_in;
	t_list	*redir_out;
} t_cmd_block;

/*
typedef struct s_ast_node {
	t_token_type    type;
	t_dlist  	   *tokens;
	char			*value;
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;*/



typedef struct s_ast_node {
	t_token_type    type;
	char		*value;
	t_cmd_block	*cmd_block;
	t_dlist  	   *tokens;
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;



// utils
void		swap_char(char *c1, char *c2);
void		revstr(char *str);
int			is_redir(t_token_type type);
int			is_command(t_token_type type);
int			is_operator(t_token_type type);
int			verify_tokens(t_dlist *tokens);
void		print_ast(t_ast_node *root);
void		print_tree(t_ast_node *node, int depth);
//attention c'est de la merde
void		print_tree_2(t_ast_node *node, int depth);
void		print_token_type(t_token_lst *token);


// lexing
void		parse_input(char *input, t_token_lst **token_lst);
int			same_quote(t_tkn_info *tkn_info);
void		lexer(char	*input, t_token_lst **tokens);
t_token_lst	*token_new(t_token_type type, void *content);
t_token_lst	*token_last(t_token_lst *token_lst);
void		tokenaddback(t_token_lst **lst, t_token_lst *node);
t_token_lst	*redir_token(char *str);
t_dlist		*tokenize(char *input);

// parsing
void		parser(t_token_lst *tokens);
int			ft_isvalidchar(char c);
t_ast_node	*parse_tokens(t_dlist *tokens);
t_ast_node	*construct_ast_from_tokens(t_dlist *token_list);

// expansing
void		expand_ast(t_ast_node *node);
void		fill_cmd_block(t_cmd_block *block, t_dlist *tokens);
t_cmd_block	*init_cmd_block(void);
void	print_cmd_block(t_cmd_block *cmd_block);

// signal
void		setup_signal_handlers(void);
void		handle_sigint(int sig);


// memory
void		free_tokens(t_token_lst	*token);
void		clear_cmd_block(t_cmd_block *block);

// execution

//dirs_handling
void	pwd(void);
void	act_env_pwd(char **env);
int 	change_directory(char *path, char **env);
char	*get_cwd(void);

//env
void	export(char ***env, char **arg);
void	unset(char ***env, char **args);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*get_env_var(char **env, char *var_id);

#endif
