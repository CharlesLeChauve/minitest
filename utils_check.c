#include "minishell.h"

int    is_redir(t_token_type type)
{
    if (type == redir_app || type == redir_in || type == redir_out || type == heredoc)
        return (1);
    return (0);
}
