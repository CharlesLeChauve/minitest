#include "minishell.h"

int	is_redir(t_token_type type)
{
	if (type == redir_app || type == redir_in || type == redir_out || \
		type == heredoc)
		return (1);
	return (0);
}

int	is_command(t_token_type type)
{
	if (type == command || type == subshell)
		return (1);
	return(0);
}

int	is_operator(t_token_type type)
{
	return (type == and_op || type == or_op || type == pipe_op);
}
