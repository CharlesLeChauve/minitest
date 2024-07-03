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
