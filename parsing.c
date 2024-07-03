/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*   And: anporced <anporced@student.42.fr>       +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:07:26 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 15:07:26 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	incr_shlvl(char **var)
{
	int		i;
	int		lvl;
	char	*str_lvl;

	i = 0;
	while ((*var)[i])
		i++;
	i--;
	while (ft_isdigit((*var)[i]))
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
	int		i;
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

char	*build_prompt(void)
{
	char	*pwd;
	char	*prompt;

	pwd = get_cwd();
	if (pwd == NULL)
		return ("Minishell_project");
	prompt = ft_strdup(TASH_PROMPT_S);
	prompt = ft_strjoin_free(prompt, pwd, 2);
	prompt = ft_strjoin_free(prompt, TASH_PROMPT_E, 0);
	return (prompt);
}

char	*prompted_readline(void)
{
	char	*input;
	//char *prompt;

	//prompt = build_prompt();
	input = readline("minishell ");
	//free(prompt);
	return (input);
}

void	del_tkn_node(void *node_ptr)
{
	t_token_lst	*node;

	if (!node_ptr)
		return ;
	node = (t_token_lst *)node_ptr;
	if (node->text)
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
	clear_cmd_block(ast->cmd_block);
	ft_dlstclear(&(ast->tokens), del_tkn_node);
	if (ast->value != NULL)
		free(ast->value);
	free(ast);
}

void	destroy_heredocs(void)
{
	int		hd_1;
	int		hd_2;
	int		found;
	char	path[15];

	hd_1 = 0;
	while (1)
	{
		hd_2 = 0;
		ft_bzero(path, 15);
		ft_sprintf(path, ".heredoc%d.%d", hd_1, hd_2);
		found = 0;
		while (access(path, W_OK) == 0)
		{
			unlink(path);
			found++;
			hd_2++;
			ft_bzero(path, 15);
			ft_sprintf(path, ".heredoc%d.%d", hd_1, hd_2);
		}
		if (!found)
			break ;
		hd_1++;
	}
}

void	clean_shell_instance(t_shell *shl)
{
	if (shl->ast)
		free_ast(shl->ast);
	destroy_heredocs();
	shl->ast = NULL;
	shl->token_lst = NULL;
}

int	treat_input(char *input, t_shell *shl, int *verif)
{
	add_history(input);
	shl->token_lst = tokenize(input);
	*verif = verify_tokens(shl->token_lst);
	if (!*verif)
	{
		ft_dlstclear(&(shl->token_lst), del_tkn_node);
		clean_shell_instance(shl);
		shl->last_ret = 2;
		return (1);
	}
	shl->ast = parse_tokens(shl->token_lst);
	if (!shl->ast || expand_ast(shl->ast, shl))
	{
		shl->last_ret = 1;
		clean_shell_instance(shl);
		return (1);
	}
	shl->last_ret = exec_ast(shl->ast, shl);
	clean_shell_instance(shl);
	return (0);
}

int	get_and_treat_input(t_shell *shl, int *verif)
{
	char	*input;

	input = prompted_readline();
	if (!input)
		return (1);
	if (input && *input)
	{
		if (treat_input(input, shl, verif))
		{
			free(input);
			return (2);
		}
	}
	free(input);
	return (0);
}

int	ft_subshell(char *input, char *envp[])
{
	t_shell	shl;
	int		verif;

	shl.env = set_env(envp);
	shl.last_ret = 0;
	shl.ast = NULL;
	verif = 0;
	if (input && *input)
	{
		if (treat_input(input, &shl, &verif))
		{
			free(input);
			return (2);
		}
	}
	ft_free_tab(shl.env);
	return (shl.last_ret);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shl;
	int		verif;
	int		status;

	if (argc > 1)
		printf("Usage : %s (No args)\n", argv[0]);
	signal(SIGQUIT, SIG_IGN);
	setup_signal_handlers();
	shl.env = set_env(envp);
	shl.last_ret = 0;
	shl.ast = NULL;
	verif = 0;
	while (1)
	{
		status = get_and_treat_input(&shl, &verif);
		if (status == 1)
			break ;
		if (status == 2)
			continue ;
	}
	ft_free_tab(shl.env);
	return (0);
}
