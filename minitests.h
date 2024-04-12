#include "libft/libft.h"

typedef enum {
	end_of_file, s_quotes, d_quotes
} t_symbol;

typedef enum {
	command, option, argunent, redirection, pipe
}	t_type;

typedef struct s_syntax_node {
	t_type	type;
	char	**content;
	struct s_syntax_node *left;
	struct s_syntax_node *right;
}				t_syntax_node;


