#include "minishell.h"

int	handle_asterisk(char **pattern)
{
	while (**pattern == '*')
		(*pattern)++;
	return (!**pattern);
}

int	match_char(char **str, char **pattern, char **s_ptr, char **p_ptr)
{
	if (**pattern == '?' || **pattern == **str)
	{
		(*str)++;
		(*pattern)++;
	}
	else if (*s_ptr)
	{
		(*s_ptr)++;
		*str = *s_ptr;
		*pattern = *p_ptr;
	}
	else
		return (0);
	return (1);
}

int	match_pattern(char *str, char *pattern)
{
	char	*s_ptr;
	char	*p_ptr;

	p_ptr = NULL;
	s_ptr = NULL;
	if (str[0] == '.' && pattern[0] != '.')
		return (0);
	while (*str)
	{
		if (*pattern == '*')
		{
			if (handle_asterisk(&pattern))
				return (1);
			s_ptr = str;
			p_ptr = pattern;
		}
		else if (!match_char(&str, &pattern, &s_ptr, &p_ptr))
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
	if (!text[i])
		return (i);
	else
		return (i + 1);
}

char	*get_stash(char *text)
{
	int		index;
	char	*stash;

	index = get_next_slash(text);
	if (index == -1)
		return (NULL);
	if (index == 0)
		return (text);
	stash = &text[index];
	return (stash);
}

char	*get_first_dir(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '/')
		i++;
	return (ft_strndup(str, i));
}

void	ft_lstinsert_after(t_list **target, t_list *node)
{
	if (!target || !node)
		return ;
	if (*target == NULL)
	{
		*target = node;
		return ;
	}
	node->next = (*target)->next;
	(*target)->next = node;
}

void	ft_lstinsert_before(t_list **target, t_list *node)
{
	t_list	*tmp;

	if (!target || !node)
		return ;
	if (*target == NULL)
	{
		*target = node;
		return ;
	}
	tmp = (t_list *)malloc(sizeof(t_list));
	if (!tmp)
		return ;
	tmp->content = (*target)->content;
	tmp->next = (*target)->next;
	(*target)->content = ft_strdup(node->content);
	(*target)->next = tmp;
}

void	ft_lstinsert_lst_replace(t_list **target, t_list *lst)
{
	t_list	*last_node;
	t_list	*temp;

	if (!target || !lst)
		return ;
	last_node = ft_lstlast(lst);
	if (*target == NULL)
		*target = lst;
	else
	{
		temp = (*target)->next;
		free((*target)->content);
		(*target)->content = lst->content;
		(*target)->next = lst->next;
		free(lst);
		last_node->next = temp;
	}
}

DIR *open_directory(char *prefix)
{
	if (!prefix || !*prefix)
		return (opendir("./"));
	else
		return (opendir(prefix));
}

char *get_evaluation(char *suffix, char **stash)
{
	*stash = get_stash(suffix);
	if (*stash)
		return (get_first_dir(suffix));
	else
		return (suffix);
}

void read_directory(DIR *dir, char *prefix, char *eval, \
	char *stash, t_list **results)
{
	struct dirent    *ent;
	char            *res;

	ent = readdir(dir);
	while (ent != NULL)
	{
		if (match_pattern(ent->d_name, eval))
		{
			res = ft_strjoin(prefix, ent->d_name);
			if (stash && stash[0])
			{
				res = ft_strjoin_free(res, "/", 0);
				expand_wildcard(res, stash, results);
				free(res);
			}
			else
				ft_lstadd_back(results, ft_lstnew(res));
		}
		ent = readdir(dir);
	}
}

void    expand_wildcard(char *prefix, char *suffix, t_list **arg_lst)
{
	DIR     *dir;
	char    *stash;
	char    *eval;
	t_list  *results;

	results = NULL;
	if (!suffix)
		return;
	dir = open_directory(prefix);
	if (!dir)
		return;
	eval = get_evaluation(suffix, &stash);
	read_directory(dir, prefix, eval, stash, &results);
	free(eval);
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

int    simple_expand(char *prefix, char *suffix, char **str)
{
	DIR     *dir;
	char    *stash;
	char    *eval;
	t_list  *results;

	results = NULL;
	if (!suffix)
		return (0);
	dir = open_directory(prefix);
	if (!dir)
		return (1);
	eval = get_evaluation(suffix, &stash);
	read_directory(dir, prefix, eval, stash, &results);
	free(eval);
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
