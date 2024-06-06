#include "minishell.h"

void ft_echo(char **args)
{
	int	i;
	int	newline;
	int	valid_option;
	int	j;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		valid_option = 1;
		while (args[i][j])
		{
			if (args[i][j] != 'n')
			{
				valid_option = 0;
				break ;
			}
			j++;
		}
		if (valid_option)
		{
			newline = 0;
			i++;
		}
		else
			break ;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
