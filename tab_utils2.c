#include "minishell.h"

char **ft_cpy_strtab(char **tab)
{
	int		i;
	char	**env_cpy;
	
	i = 0;
	while (tab[i])
		i++;
	env_cpy = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (tab[i])
	{
		env_cpy[i] = ft_strdup(tab[i]);
		i++;
	}
	env_cpy[i] = NULL;
	return (env_cpy);
}
