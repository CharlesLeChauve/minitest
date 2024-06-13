#include "minishell.h"

void	print_export_env(char **env)
{
	int	i;

	i = 0;
	ft_sort_wordtab(env);
	while (env[i])
	{
		if (strncmp(env[i], "_=", 2) == 0)
		{
			i++;
			continue ;
		}
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
}

char	*export_format(char *str)
{
	char *formated;
	int	i;
	int	j;

	i = 0;
	j = 0;
	formated = (char *)malloc(ft_strlen(str) + 3);
	while (str[i] && str[i - 1] != '=')
		formated[j++] = str[i++];
	if (str[i] == '\0' && !(str[i - 1] == '='))
		return (formated);
	formated[j++] = '"';
	while (str[i])
	{
		formated[j] = str[i];
		i++;
		j++;
	}
	formated[j++] = '"';
	formated[j++] = '\0';
	return (formated);
}

//export OK
//Usage : Premier argument est un pointeur vers les tableau de chaines de caracteres env
// Deuxieme argument un tableau NULL termine de chaines de caracteres NULL terminees listant les variables a ajouter;
int    export(char **env[], char **arg)
{
	char	**env_cpy;
	int		i;

	i = 0;
	if (*arg == NULL)
	{
		while ((*env)[i])
			i++;
		env_cpy = (char **)malloc((i + 1) * sizeof(char *));
		i = -1;
		while ((*env)[++i])
			env_cpy[i] = export_format((*env)[i]);
		env_cpy[i] = NULL;
		print_export_env(env_cpy);
	}
	else
	{
		replace_existing_vars(&arg, env);
		add_strs_to_strtab(arg, env);
	}
	return (0);
}