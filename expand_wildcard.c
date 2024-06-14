# include "minishell.h"

int	match_pattern(char *str, char *pattern)
{
	char	*s_ptr;
	char	*p_ptr;

	p_ptr = NULL;
	s_ptr = NULL;
	while (*str)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			s_ptr = str;
			p_ptr = pattern;
		}
		else if (*pattern == '?' || *pattern == *str)
		{
			str++;
			pattern++;
		}
		else if (s_ptr)
		{
			s_ptr++;
			str = s_ptr;
			pattern = p_ptr;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern);
}

int	get_last_full_dir_idx(char *ptr)
{
	int		idx;
	int		i;

	idx = 0;
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == '/')
			idx = i;
		if (ptr[i] == '*' || ptr[i] == '?')
			return (idx);
		i++;
	}
	return (-1);
}

int	get_next_slash(char *text)
{
	int	i;

	i = 0;
	while (text[i] && text[i] != '/')
	{
		i++;
	}
	return (i);
}

char *get_stash(char *text)
{
	int	index;
	char	*stash;

	index = get_next_slash(text);
	printf("index = %d\n", index);
	if (index == -1)
		return (NULL);
	if (index == 0)
		return (text);
	stash = ft_substr(text, index, ft_strlen(text) - index);
	return (stash);
}
char *get_first_dir(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '/')
		i++;
	return (ft_strndup(str, i));
}

void	expand_wildcard(char *prefix, char *suffix, t_list **arg_lst)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*stash;
	char			*eval;
	char			*res;
	char			*new_prefix;

	dir = NULL;
	if (!suffix)
		return ;
	if (!prefix || !*prefix)
		new_prefix = ft_strdup("./");
	else
		new_prefix = prefix;
	stash = NULL;
	dir = opendir(new_prefix);
	if (!dir)
		perror("dir");
	ent = readdir(dir);
	stash = get_stash(suffix);
	if (stash)
	{
		ft_printf("Allo\n");
		eval = get_first_dir(suffix);
		ft_printf("eval = %s\n",eval);
	}
	else
		eval = suffix;
	ent = readdir(dir);
	while (ent != NULL)
	{
		ft_printf("ent_name = %s, eval = %s\n", ent->d_name, eval);
		if (match_pattern(ent->d_name, eval))
		{
			ft_printf("Matched ! : %s\n", ent->d_name);
			res = ft_strjoin_free(prefix, ent->d_name, 0);
			if (stash)
				expand_wildcard(res, stash, arg_lst);
			else
				ft_lstadd_back(arg_lst, ft_lstnew(res));
		}
		ent = readdir(dir);
	}
	closedir(dir);
}

void	expand_wildcards_in_block(t_cmd_block *block)
{
	t_list	*arg;
	char	*suffix;
	char	*prefix;
	int     index;
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
		}
		arg = next;
	}
}
