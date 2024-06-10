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

char *prompted_readline(char *env[])
{
	char *prompt;
	char *input;

	prompt = build_prompt(env);
	input = readline(prompt);
	free(prompt);
	return (input);
}

void	del_tkn_node(void *node_ptr)
{
	t_token_lst *node;

	node = (t_token_lst *)node_ptr;
	free(node->text);
	node->text = NULL;
	free(node);
}

void	free_ast(t_ast_node *ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->value != NULL)
		free(ast->value);
	free(ast);
}

void	clean_shell_instance(t_shell *shl)
{
	// free_ast(shl->ast);
	shl->ast = NULL;
	ft_dlstclear(&(shl->token_lst), del_tkn_node);
	shl->token_lst = NULL;
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_shell	shl;

	setup_signal_handlers();
	input = NULL;
	shl.env = set_env(envp);
	shl.last_ret = 0;
	while (1)
	{
		printf("Last_return_value = %d\n", shl.last_ret);
		if (input)
			input = ft_strjoin_free(input, readline("> "), 1);
		else
			input = prompted_readline(shl.env);
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			shl.token_lst = tokenize(input);
			if (verify_tokens(shl.token_lst) == -1)
				continue ;
			else if (!verify_tokens(shl.token_lst))
			{
				fprintf(stderr, "Error: Syntax error in input\n");
				free(input);
				shl.last_ret = 2;
				input = NULL;
				continue ;
			}
			shl.ast = parse_tokens(shl.token_lst);
			//expand_ast(shl.ast);
			//print_tree(ast, 0);
			shl.last_ret = exec_ast(shl.ast, &shl);
			clean_shell_instance(&shl);
		}
		free(input);
		input = NULL;
	}
	ft_free_tab(shl.env);
}
