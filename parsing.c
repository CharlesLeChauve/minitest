#include "minishell.h"

int	ft_isvalidchar(char c)
{
	if (c == '.' || c == '/' || ft_isalpha(c) || c == '*' || c == '?')
		return (1);
	return (0);
}

void	swap_char(char *c1, char *c2)
{
	char	tmp;

	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

void	revstr(char *str)
{
	unsigned int	size;
	unsigned int	i;

	size = ft_strlen(str);
	i = -1;
	while (++i < --size)
		swap_char(&str[i], &str[size]);
}

void	parse_input(char *input, t_token_lst **token_lst)
{
	char	**input_tab;
	int	i;

	input_tab = ft_split(input, ' ');
	i = 0;
	while (input_tab[i])
	{
		treat_token(input_tab[i], token_lst);
		i++;
	}
}

int	main(void)
{
	char			*input;
	struct sigaction	sa;
	t_token_lst			*token_lst;
	t_ast_node			*ast;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	token_lst = NULL;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		input = readline("Maxishell > ");
		if (!input)
			break;
		if (input && *input)
		{
			add_history(input);
			tokenize(input);
			ast = parse_tokens(token_lst);
			print_tree(ast);
			// revstr(input);
			// printf("%s\n", input);
		}
		token_lst = NULL;
		free(input);
	}
}
