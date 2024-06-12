# include "minishell.h"

int	match_pattern(char *str, char *pattern)
{
	char	*s_ptr;
	char	*p_ptr;

	p_ptr = NULL;
	while (*str)
	{
		if (pattern == '*')
		{
			s_ptr = str;
			p_ptr = pattern;
		}
		else if (pattern == '?' || *pattern == *str)
		{
			str++;
			pattern++;
		}
		else if (p_ptr)
		{
			str++;
			pattern = p_ptr;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern);
}

void	expand_wildcard(char *prefix, char *suffix)
{
	DIR				*dir;
	struct dirent	*ent;

	if (prefix == NULL)
		prefix = ft_strdup("./");
	dir = opendir(prefix);
	ent = readdir(dir);
	while (ent != NULL)
	{
		ent = readdir(dir);
		if ()
	}
	
}

void	expand_wildcards_in_block(t_cmd_block *block)
{
	t_list *arg;
	int     index;

	index = 0;
	expand_wildcard(NULL, block->command);
	arg = block->arg;
	while (arg)
	{
		expand_args_wildcard(arg, block, index);
	}
}