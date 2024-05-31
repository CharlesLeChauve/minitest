#include "minishell.h"

void    print_str_tab(char **tab)
{
    while (*tab)
    {
        ft_putstr_fd(*tab, 1);
        write(1, "\n", 1);
        tab++;
    }
}

void	ft_sort_wordtab(char **tab)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (tab[i] != 0)
	{
		j = i + 1;
		while (tab[j] != 0)
		{
			if (ft_strcmp(tab[i], tab[j]) > 0)
			{
				temp = tab[i];
				tab[i] = tab[j];
				tab[j] = temp;
			}
			++j;
		}
		++i;
	}
}

void	ft_remove_from_strtab(char **tab, int index)
{
	int	size;

	size = 0;
	while (tab[size])
		size++;
	free(tab[index]);
	while (index < size - 1)
	{
		tab[index] = tab[index + 1];
		index++;
	}
	tab[size - 1] = NULL;
}
