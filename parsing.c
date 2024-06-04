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

void	incr_shlvl(char **var)
{
	int		i;
	int 	lvl;
	char	*str_lvl;

	i = 0;
	while ((*var)[i])
		i++;
	i--;
	while(ft_isdigit((*var)[i]))
		i--;
	i++;
	lvl = ft_atoi(&(*var)[i]);
	lvl++;
	str_lvl = ft_itoa(lvl);
	while ((*var)[i])
		(*var)[i++] = '\0';
	*var = ft_strjoin_free(*var, str_lvl, 2);
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
		if (envp[i] == get_env_var(envp, "SHLVL"))
			incr_shlvl(&env[i]);
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

char	*build_prompt(char **env)
{
	char	*pwd;
	char	*pwd_ptr;
	char	*prompt;
	int		len;

	pwd = get_cwd();
	if (pwd == NULL)
		return ("Minishell_project");
	prompt = ft_strdup(TASH_PROMPT_S);
	prompt = ft_strjoin_free(prompt, pwd, 2);
	prompt = ft_strjoin_free(prompt, TASH_PROMPT_E, 0);
	return (prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	t_ast_node			*ast;
	char				**env;
	t_dlist				*token_lst;
	char				*prompt;
	int					in_heredoc;

	in_heredoc = 0;
	setup_signal_handlers();
	input = NULL;
	env = set_env(envp);
	while (1)
	{
		if (input)
			input = ft_strjoin_free(input, readline("> "), 1);
		else
		{
			prompt = build_prompt(env);
			input = readline(prompt);
		}
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			token_lst = tokenize(input);
			if (verify_tokens(token_lst) == -1)
				continue ;
			else if (!verify_tokens(token_lst))
			{
				fprintf(stderr, "Error: Syntax error in input\n");
				free(input);
				input = NULL;
				continue ;
			}
			ast = parse_tokens(token_lst);
			expand_ast(ast);
			//print_tree(ast, 0);
			exec_ast(ast, &env);
			ast = NULL;
		}
		free(input);
		input = NULL;
	}
	ft_free_tab(env);
}
