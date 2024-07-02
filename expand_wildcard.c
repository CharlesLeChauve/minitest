/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:48:51 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 15:03:31 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

DIR	*open_directory(char *prefix)
{
	if (!prefix || !*prefix)
		return (opendir("./"));
	else
		return (opendir(prefix));
}

void	read_directory(DIR *dir, char *prefix, \
	t_wc_strs *strs, t_list **results)
{
	struct dirent	*ent;
	char			*res;

	ent = readdir(dir);
	while (ent != NULL)
	{
		if (match_pattern(ent->d_name, strs->eval))
		{
			res = ft_strjoin(prefix, ent->d_name);
			if (strs->stash && strs->stash[0])
			{
				res = ft_strjoin_free(res, "/", 0);
				expand_wildcard(res, strs->stash, results);
				free(res);
			}
			else
				ft_lstadd_back(results, ft_lstnew(res));
		}
		ent = readdir(dir);
	}
}

void	expand_wildcard(char *prefix, char *suffix, t_list **arg_lst)
{
	DIR			*dir;
	t_wc_strs	strs;
	t_list		*results;

	results = NULL;
	if (!suffix)
		return ;
	dir = open_directory(prefix);
	if (!dir)
		return ;
	strs.eval = get_evaluation(suffix, &strs.stash);
	read_directory(dir, prefix, &strs, &results);
	free(strs.eval);
	closedir(dir);
	ft_sort_wordlist(&results);
	ft_lstinsert_lst_replace(arg_lst, results);
}

void	expand_wildcards_in_block(t_cmd_block *block)
{
	t_list	*arg;
	char	*suffix;
	char	*prefix;
	int		index;
	t_list	*next;

	arg = block->arg;
	while (arg)
	{
		next = arg->next;
		index = get_last_full_dir_idx(arg->content);
		if (index >= 0)
		{	
			prefix = ft_strndup(arg->content, index);
			suffix = ft_strdup(arg->content + index);
			expand_wildcard(prefix, suffix, &arg);
			free(prefix);
			prefix = NULL;
			free (suffix);
			suffix = NULL;
		}
		arg = next;
	}
}

int	simple_expand(char *prefix, char *suffix, char **str)
{
	DIR			*dir;
	t_wc_strs	strs;
	t_list		*results;

	results = NULL;
	if (!suffix)
		return (0);
	dir = open_directory(prefix);
	if (!dir)
		return (1);
	strs.eval = get_evaluation(suffix, &strs.stash);
	read_directory(dir, prefix, &strs, &results);
	free(strs.eval);
	closedir(dir);
	if (ft_lstsize(results) > 1)
	{
		ft_putstr_fd(" ambiguous redirect\n", 2);
		return (1);
	}
	if (results)
		*str = results->content;
	return (0);
}
