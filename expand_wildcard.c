# include "minishell.h"

int ft_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && ft_tolower(*s1) == ft_tolower(*s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2);
}

void ft_lstadd_sorted(t_list **head, t_list *new_node, int (*cmp)(const char*, const char*))
{
    t_list *current;

    if (*head == NULL || cmp((char *)(*head)->content, (char *)new_node->content) > 0)
    {
        new_node->next = *head;
        *head = new_node;
    }
    else
    {
        current = *head;
        while (current->next != NULL && cmp((char *)current->next->content, (char *)new_node->content) <= 0)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void ft_sort_wordlist(t_list **head)
{
    t_list *sorted = NULL;
    t_list *current = *head;
    t_list *next_node;

    while (current != NULL)
    {
        next_node = current->next;
        ft_lstadd_sorted(&sorted, current, ft_strcasecmp);
        current = next_node;
    }
    *head = sorted;
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
	if (!text[i])
		return (i);
	else
		return (i + 1);
}

char *get_stash(char *text)
{
	int	index;
	char	*stash;

	index = get_next_slash(text);
	if (index == -1)
		return (NULL);
	if (index == 0)
		return (text);
	stash = &text[index]; //ft_substr(text, index, ft_strlen(text) - index);
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

void	ft_lstinsert_after(t_list **target, t_list *node)
{
	if (!target || !node)
		return;
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
	t_list *tmp;

	if (!target || !node)
		return;
	if (*target == NULL)
	{
		*target = node;
		return ;
	}
	tmp = (t_list *)malloc(sizeof(t_list));
	if (!tmp)
		return;
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
		return;
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


void	expand_wildcard(char *prefix, char *suffix, t_list **arg_lst)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*stash;
	char			*eval;
	char			*res;
	t_list			*results;

	dir = NULL;
	results = NULL;
	if (!suffix)
		return ;
	if (!prefix || !*prefix)
		dir = opendir("./");
	else	
		dir = opendir(prefix);
	stash = NULL;
	if (!dir)
		return ;
	stash = get_stash(suffix);
	if (stash)
		eval = get_first_dir(suffix);
	else
		eval = suffix;
	ent = readdir(dir);
	while (ent != NULL)
	{
		if (match_pattern(ent->d_name, eval))
		{
			res = ft_strjoin(prefix, ent->d_name);
			if (stash && stash[0])
			{
				res = ft_strjoin_free(res, "/", 0);
				expand_wildcard(res, stash, arg_lst);
				free(res);
			}
			else
				ft_lstadd_back(&results, ft_lstnew(res));
		}
		ent = readdir(dir);
	}
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
			free(prefix);
			prefix = NULL;
			free (suffix);
			suffix = NULL;
		}
		arg = next;
	}
}
