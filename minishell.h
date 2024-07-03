/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:44:46 by anporced          #+#    #+#             */
/*   Updated: 2024/07/03 16:44:46 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define CWD_BUFF 256
# define TASH_PROMPT_S "\001\033[0;92m\002tash \001\033[0;94m\002"
# define TASH_PROMPT_E "\001\033[0;93m\002 > \001\033[0;37m\002"

extern volatile sig_atomic_t	g_interrupted;

typedef enum e_token_type
{
	command,
	redir_in,
	redir_out,
	redir_app,
	heredoc,
	pipe_op,
	fd,
	file_path,
	and_op,
	or_op,
	subshell,
	eol
}			t_token_type;

typedef enum e_state_machine
{
	reg,
	quote,
	dquote
}	t_sm;

typedef enum e_open_mode
{
	append_o,
	truncate_o,
	read_o
}	t_open_mode;

typedef struct s_heredoc
{
	char				*hd;
	int					tty_fd;
	int					fd;
	struct sigaction	old_sa;
}						t_heredoc;

typedef struct s_wc_strs
{
	char	*eval;
	char	*stash;
}			t_wc_strs;

typedef struct s_std_fd_save
{
	int	std_out;
	int	std_in;
	int	std_err;
}		t_std_fd_save;

typedef struct s_pipe_infos
{
	int		pipe_fds[2];
	int		pids[2];
}	t_pipe_info;

typedef struct s_token_lst
{
	t_token_type	type;
	char			*text;
}					t_token_lst;

typedef struct s_tkn_info
{
	t_dlist		*token_lst;
	t_sm		state;
	char		*input;
	char		*curr_char;
}				t_tkn_info;

typedef struct s_params
{
	size_t	len;
	int		quote;
	t_sm	state;
}			t_params;

typedef struct s_cmd_block
{
	char	**exec_tab;
	char	*command;
	t_list	*arg;
	t_list	*redirs;
}			t_cmd_block;

typedef struct s_ast_node
{
	t_token_type		type;
	char				*value;
	t_cmd_block			*cmd_block;
	t_dlist				*tokens;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

typedef struct s_shell
{
	t_dlist		*token_lst;
	t_ast_node	*ast;
	int			last_ret;
	char		**env;
}				t_shell;

// utils
void				swap_char(char *c1, char *c2);
void				revstr(char *str);
int					is_redir(t_token_type type);
int					is_command(t_token_type type);
int					is_operator(t_token_type type);
int					verify_tokens(t_dlist *tokens);
void				print_ast(t_ast_node *root);
void				print_tree(t_ast_node *node, int depth);
void				print_tree_2(t_ast_node *node, int depth);
void				print_token_type(t_token_lst *token);
int					ft_isshelloperator(char c);

// lexing
int					break_token(t_tkn_info *tkn_info);
void				set_token_text(t_tkn_info *tkn_info, t_token_lst *token);
t_token_lst			*cmd_token(t_tkn_info *tkn_info);
t_token_lst			*handle_pipe(t_tkn_info *tkn_info);
t_token_lst			*handle_ampersand(t_tkn_info *tkn_info);
void				init_tkn_info(t_tkn_info *tkn_info, char *input);
int					ft_isshelloperator(char c);
void				set_quotes_state(t_tkn_info *tkn_info);
void				first_read_quotes(t_tkn_info *tkn_info);
void				space_quotes(t_tkn_info *tkn_info);
int					same_quote(t_sm *state, char c);
void				read_subshell(t_tkn_info *tkn_info, char **buffer, \
size_t *len, int *par_lvl);
t_token_lst			*subshell_token(t_tkn_info *tkn_info);
t_token_lst			*handle_paren(t_tkn_info *tkn_info);
t_token_lst			*token_new(t_token_type type, void *content);
t_token_lst			*token_last(t_token_lst *token_lst);
void				tokenaddback(t_token_lst **lst, t_token_lst *node);
char				*redir_token(char **str, t_sm *state);
t_dlist				*tokenize(char *input);
void				ft_add_char_to_buffer(char **buffer, char c, size_t *len);

// parsing
void				handle_memory_error(void);
void				initialize_command_node(t_ast_node *node,
						char *text, t_dlist *tokens);
void				initialize_operator_node(t_ast_node *node);
void				parser(t_token_lst *tokens);
t_ast_node			*parse_tokens(t_dlist *tokens);
t_ast_node			*construct_ast_from_tokens(t_dlist *token_list);

// expansing
t_cmd_block			*init_cmd_block(void);
int					same_quote(t_sm *state, char c);
void				handle_state(char **ptr, t_params *params,
						char **buffer, t_shell *shell);
char				*handle_redirection_or_not(char **ptr, t_params *params,
						int *is_a_redir, char *buffer);
char				*extract_command(char **ptr, t_shell *shell,
						int *is_a_redir);
int					handle_redirection(char *sub_token, t_cmd_block *block,
						t_token_type type);
int					process_sub_token(char *sub_token, t_cmd_block *block,
						int is_a_redir);
int					ft_isrediroperator(char c);
void				set_quotes_state_in_redir(char **curr_char, t_sm *state,
						int *quotectrl);
t_token_type		redir_type(char *curr_char);
void				handle_quotes_and_operators(char **curr_char, t_sm *state,
						char **buffer, t_params *params);
char				*handle_redirection_or_not(char **ptr, t_params *params,
						int *is_a_redir, char *buffer);
void				skip_whitespace(char **curr_char, t_sm *state,
						char **buffer, size_t *len);
int					handle_syntax_error(char **buffer, int quote);
char				*extrapolate(char **str, t_shell *shell);
int					fill_cmd_block(t_cmd_block *block, t_dlist *tokens,
						t_shell *shell);
void				print_cmd_block(t_cmd_block *cmd_block);
int					expand_ast(t_ast_node *node, t_shell *shl);

// signal
void				setup_signal_handlers(void);
void				handle_sigint(int sig);
void				handle_main_signal(void);
void				handle_heredoc_signal(void);

// memory
void				free_tokens(t_token_lst	*token);
void				clear_cmd_block(t_cmd_block *block);

// execution

//dirs_handling
void				pwd(void);
void				act_env_pwd(char ***env);
int					change_directory(char **args, char ***env);
char				*get_cwd(void);

//env
int					export(char ***env, char **arg);
int					unset(char ***env, char **args);
int					no_equal(char *var);
void				*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int					print_env(char **env);

//env_utils.c
char				*get_env_var(char **env, char *var_id);
int					get_env_index(char **env, char *var_id);

//exec.c
int					exec_ast(t_ast_node *ast, t_shell *shl);
void				create_exec_tab(t_cmd_block *cmd_block);

t_ast_node			*build_ast(t_dlist **token);
void				replace_var(char ***env, char *new_var, char *old_var);
void				replace_existing_vars(char ***arg, char ***env);

//pipes
int					handle_pipes(t_ast_node *ast, t_shell *shl);

//tab_utils.c
void				ft_sort_wordtab(char **tab);
void				restore_stds_and_close_dup(int in_save, int out_save,
						int err_save);
void				remove_from_tab(char ***tab, int index, int free_ctl);
void				add_strs_to_strtab(char **strs, char ***strtab);

//redirs.c
int					handle_redirs(t_cmd_block *cmd_block);
int					make_redir_in(t_token_lst *redir);

//path_utils.c
char				*set_cmd_path(char *envp[], char *cmd);

//file_utils.c
int					check_acces(char *file, t_open_mode mode);
int					open_mode(char *path, t_open_mode mode);
int					open_write(char *file, t_open_mode mode);

//exec_actions.c
int					do_the_builtin(t_shell *shl, char **env[],
						char *cmd, char **cmd_tab);
int					is_a_builtin(char *command);

//heredoc.c
int					heredoc_handle(char *limiter, t_std_fd_save save);

int					wait_status(pid_t pid);
int					ft_is_numeric(char *str);
int					ft_exit(char **args, t_shell *shl);

//echo.c
void				ft_echo(char **args);

//exec_utils.c
void				exit_exec(t_shell *shl, int ex_no);
void				exec_error(char *str, int mode);
void				exec_path_access(t_shell *shl, char *cmd);

//type_utils
int					is_redir(t_token_type type);
int					is_command(t_token_type type);
int					is_operator(t_token_type type);

//wildcards_get_utils.c
char				*get_stash(char *text);
int					get_next_slash(char *text);
char				*get_first_dir(char *str);
int					get_last_full_dir_idx(char *ptr);
char				*get_evaluation(char *suffix, char **stash);

//export_utils.c
int					check_export_arg(char *arg);
void				print_export_env(char **env);
char				*export_format(char *str);

//heredoc_signal.c
void				restore_signal_handlers(struct sigaction old_sa);
struct sigaction	*setup_signal_handlers_h(struct sigaction *old_sa);

//heredoc_utils.c
int					handle_interruption(char *nl, t_heredoc *heredoc);
int					handle_end_of_file(t_heredoc *heredoc);
int					handle_match_limiter(char *nl, t_heredoc *heredoc,
						char *limiter);
int					open_tty(int *fd);

int					match_pattern(char *str, char *pattern);

int					get_heredocs(t_cmd_block *cmd_block);

void				expand_wildcards_in_block(t_cmd_block *block);
void				expand_wildcard(char *prefix, char *suffix,
						t_list **arg_lst);
int					simple_expand(char *prefix, char *suffix, char **str);

void				del_tkn_node(void *node_ptr);

void				clean_shell_instance(t_shell *shl);

int					is_logical(t_token_type type);

void				ft_sort_wordlist(t_list **head);

int					ft_subshell(char *input, char *envp[]);
char				**ft_cpy_strtab(char **tab);
char				*extract_var_id(char *arg);
int					get_last_full_dir_idx(char *ptr);
int					path_start(char *str);
char				*prompted_readline(void);
char				**set_env(char **envp);

#endif
