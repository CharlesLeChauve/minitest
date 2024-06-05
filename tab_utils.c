#include "minishell.h"

void remove_from_tab(char ***tab, int index)
{
	char **temp;

	if (tab == NULL || *tab == NULL || (*tab)[index] == NULL)
		return ;

	temp = *tab;

	int i = index;
	while (temp[i + 1])
	{
		temp[i] = temp[i + 1];
		i++;
	}
	temp[i] = NULL;

	*tab = temp; 
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = NULL;
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		perror("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	if (!ptr)
		return (new_ptr);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

void	print_str_tab(char **tab)
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

void	add_strs_to_strtab(char **strs, char ***strtab)
{
	int	i;
	int	env_size;
	int	total_size;
	int	arg_size;

	arg_size = 0;
	env_size = 0;
	i = 0;
	while ((*strtab)[env_size])
		env_size++;
	while (strs[arg_size])
		arg_size++;
	total_size = env_size + arg_size;
	*strtab = ft_realloc(*strtab, (env_size) * sizeof (char *), (total_size + 1) * sizeof (char *));
	while (i < arg_size)
	{
		(*strtab)[env_size + i] = ft_strdup(strs[i]);
		i++;
	}
	(*strtab)[env_size + i] = NULL;
}
