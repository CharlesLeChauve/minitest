#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"

typedef enum {
	command, argument, option, redirection, pipe_op, fd, file_path
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	union {
		char	*text;
		int	fd;
	};
	struct s_token	*next;
}	t_token;