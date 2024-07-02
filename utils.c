#include "minishell.h"

int	is_logical(t_token_type type)
{
	if (type == pipe_op || type == or_op || type == and_op)
		return (1);
	return (0);
}
