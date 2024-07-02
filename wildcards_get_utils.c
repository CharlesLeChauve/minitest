#include "minishell.h"

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

char	*get_evaluation(char *suffix, char **stash)
{
	*stash = get_stash(suffix);
	if (*stash)
		return (get_first_dir(suffix));
	else
		return (suffix);
}
