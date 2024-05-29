#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <math.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include "env.h"



typedef enum e_token_type
{
	command, redir_in, redir_out, redir_app, heredoc, pipe_op, fd, file_path, and_op, or_op, subshell, eol
}	t_token_type;

typedef enum e_state_machine
{
	reg, quote, dquote
}	t_sm;

typedef enum e_open_mode
{
	append_o, truncate_o, read_o
}	t_open_mode;

typedef enum {
	COMMAND,
	PIPE,
	AND,
	OR
} NodeType;

typedef struct s_token_lst
{
	t_token_type type;
	char	*text;
}	t_token_lst;

typedef	struct	s_tkn_info
{
	t_dlist		*token_lst;
	t_sm		state;
	char		*input;
	char		*curr_char;
}	t_tkn_info;

typedef struct s_cmd_block
{
	char	**exec_tab;
	char	*command;
	t_list	*option;
	t_list	*arg;
	t_list	*redirs;
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
int			ft_isshelloperator(char c);


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
void	ft_add_char_to_buffer(char **buffer, char c, size_t *len);

// signal
void		setup_signal_handlers(void);
void		handle_sigint(int sig);


// memory
void		free_tokens(t_token_lst	*token);
void		clear_cmd_block(t_cmd_block *block);

// execution

//dirs_handling
void	pwd(void);
void	act_env_pwd(char **env, char *pwd);
int 	change_directory(char *path, char **env);
char	*get_cwd(void);

//env
int		export(char ***env, char **arg);
void	unset(char ***env, char **args);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*get_env_var(char **env, char *var_id);
void	print_env(char **env);

//exec.c
int		exec_ast(t_ast_node *ast, char **envp[]);
void 	create_exec_tab(t_cmd_block *cmd_block);


t_ast_node *build_ast(char *input);
void remove_from_tab(char ***arg, int index);

#endif
