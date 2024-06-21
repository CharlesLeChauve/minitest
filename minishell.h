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
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include "./libft/libft.h"
# include "env.h"
# define WHITE "\001\033[0;37m\002"
# define GREEN "\001\033[0;32m\002"
# define BLUE "\001\033[0;34m\002"
# define MAGENTA "\001\033[0;35m\002"
# define TASH_PROMPT_S WHITE"╭─"GREEN
# define TASH_PROMPT_E WHITE"\n\001╰─"BLUE"tash "MAGENTA"> "WHITE

extern volatile sig_atomic_t g_interrupted;

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

typedef struct s_std_fd_save
{
	int	std_out;
	int	std_in;
	int std_err;
}		t_std_fd_save;

typedef struct s_pipe_infos
{
	int		pipe_fds[2];
	int		pids[2];
}	t_pipe_info;

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
	t_list	*arg;
	t_list	*redirs;
} t_cmd_block;

typedef struct s_ast_node {
	t_token_type    type;
	char		*value;
	t_cmd_block	*cmd_block;
	t_dlist  	   *tokens;
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;

typedef struct s_shell
{
	t_dlist		*token_lst;
	t_ast_node	*ast;
	int			last_ret;
	char		**env;
}	t_shell;
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
int			same_quote(t_sm *state, char c);
void		lexer(char	*input, t_token_lst **tokens);
t_token_lst	*token_new(t_token_type type, void *content);
t_token_lst	*token_last(t_token_lst *token_lst);
void		tokenaddback(t_token_lst **lst, t_token_lst *node);
char		*redir_token(char **str, t_sm *state);
t_dlist		*tokenize(char *input);

// parsing
void		parser(t_token_lst *tokens);
int			ft_isvalidchar(char c);
t_ast_node	*parse_tokens(t_dlist *tokens);
t_ast_node	*construct_ast_from_tokens(t_dlist *token_list);

// expansing
int		expand_ast(t_ast_node *node, t_shell *shl);
int		fill_cmd_block(t_cmd_block *block, t_dlist *tokens, t_shell *shell);
t_cmd_block	*init_cmd_block(void);
void	print_cmd_block(t_cmd_block *cmd_block);
void	ft_add_char_to_buffer(char **buffer, char c, size_t *len);

// signal
void		setup_signal_handlers(void);
void		handle_sigint(int sig);
void		handle_main_signal(void);
void		handle_heredoc_signal(void);


// memory
void		free_tokens(t_token_lst	*token);
void		clear_cmd_block(t_cmd_block *block);

// execution

//dirs_handling
void	pwd(void);
void	act_env_pwd(char ***env);
int 	change_directory(char *path, char ***env);
char	*get_cwd(void);

//env
int		export(char ***env, char **arg);
int		unset(char ***env, char **args);
int		no_equal(char *var);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int		print_env(char **env);

//env_utils.c
char	*get_env_var(char **env, char *var_id);
int		get_env_index(char **env, char *var_id);

//exec.c
int		exec_ast(t_ast_node *ast, t_shell *shl);
void 	create_exec_tab(t_cmd_block *cmd_block);


t_ast_node *build_ast(char *input);
void 	remove_from_tab(char ***arg, int index);
void	replace_var(char ***env, char *new_var, char *old_var);
void	replace_existing_vars(char ***arg, char ***env);

//pipes
int	handle_pipes(t_ast_node *ast, t_shell *shl);

//tab_utils.c
void	ft_sort_wordtab(char **tab);
void    restore_stds_and_close_dup(int in_save, int out_save, int err_save);
void	remove_from_tab(char ***tab, int index);
void	add_strs_to_strtab(char **strs, char ***strtab);

//redirs.c
int    handle_redirs(t_cmd_block *cmd_block);
int    make_redir_in(t_token_lst *redir);

//path_utils.c
char    *set_cmd_path(char *envp[], char *cmd);

//file_utils.c
int	check_acces(char *file, t_open_mode mode);
int	open_mode(char *path, t_open_mode mode);
int	open_write(char *file, t_open_mode mode);

//exec_actions.c
int do_the_builtin(char **env[], char *cmd, char **cmd_tab);
int is_a_builtin(char *command);

//heredoc.c
int	heredoc_handle(char *limiter, t_std_fd_save save);

int	wait_status(pid_t pid);
int ft_exit(char **args);

//echo.c
void ft_echo(char **args);

int	get_heredocs(t_cmd_block *cmd_block);

void	expand_wildcards_in_block(t_cmd_block *block);

void	del_tkn_node(void *node_ptr);

#endif
