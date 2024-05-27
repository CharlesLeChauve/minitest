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

char	**set_env(char **envp)
{
	int     i;
	char	**env;

    i = 0;
    while (envp[i])
        i++;
    env = (char **)malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (envp[i])
    {
        env[i] = ft_strdup(envp[i]);
        i++;
    }
    env[i] = NULL;
	return (env);
}

// void	parse_input(char *input, t_token_lst **token_lst)
// {
// 	char	**input_tab;
// 	int	i;

// 	input_tab = ft_split(input, ' ');
// 	i = 0;
// 	while (input_tab[i])
// 	{
// 		treat_token(input_tab[i], token_lst);
// 		i++;
// 	}
// }

t_ast_node *build_ast(char *input)
{
	t_dlist		*token_lst;
	t_ast_node	*ast;

	token_lst = tokenize(input);
	t_dlist *tmp = token_lst;
	while (tmp)
	{
		print_token_type((t_token_lst *)(tmp->content));
		tmp = tmp->next;
	}
	//Changer cette fonction pour ajouter la possibilite d'avoir subshell > operateur > command
	//Globalement subshell partout ou il pouvait y avoir commande
	// if (!verify_tokens(token_lst))
	// {
	// 	fprintf(stderr, "Error: Syntax error in input\n");
	// 	free(input);
	// 	return (NULL);
	// }
	ast = parse_tokens(token_lst);
	return (ast);
}

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	struct sigaction	sa;
	t_ast_node			*ast;
	char				**env;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	env = set_env(envp);
	while (1)
	{
		input = readline("Maxishell > ");
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			/* token_lst = tokenize(input);
			if (!verify_tokens(token_lst))
			{
				fprintf(stderr, "Error: Syntax error in input\n");
				free(input);
				continue ;
			}
			ast = parse_tokens(token_lst); */
			ast = build_ast(input);
			expand_ast(ast);
			print_tree(ast, 0);
			exec_ast(ast, &env);
			//ast = NULL;
		}
		free(input);
	}
}
