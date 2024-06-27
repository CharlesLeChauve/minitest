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
	ft_bzero(formated, ft_strlen(str) + 3);
	while (str[i] && ( i == 0 || (str[i - 1] != '=')))
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

int	check_export_arg(char *arg)
{
	int		i;
	int		alpha;
	char	err[128];

	i = 0;
	alpha = 0;
	while (arg[i] && arg[i] != '=')
	{
		if ((i == 0 && arg[i] == '=') || (!ft_isalnum(arg[i]) && arg[i] != '_') || (alpha == 0 && ft_isdigit(arg[i])))
		{
			ft_sprintf(err, "tash: export: '%s': not a valid identifier", arg[i]);
			ft_putstr_fd(err, STDERR_FILENO);
			return (1);
		}
		if (ft_isalpha(arg[i]))
			alpha = 1;
		i++;
	}
	return (0);
}

char **build_export_env(char **env[])
{
	int		i;
	char	**env_cpy;

	i = 0;
	while ((*env)[i])
		i++;
	env_cpy = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while ((*env)[++i])
		env_cpy[i] = export_format((*env)[i]);
	env_cpy[i] = NULL;
	return (env_cpy);
}

void	add_str_to_strtab(char *str, char ***strtab)
{
	int	env_size;
	int	total_size;

	env_size = 0;
	while ((*strtab)[env_size])
		env_size++;
	total_size = env_size + 1;
	*strtab = ft_realloc(*strtab, env_size * sizeof(char *), (total_size + 1) * sizeof(char *));
	if (*strtab == NULL)
		return ;
	(*strtab)[env_size] = str;
	if ((*strtab)[env_size] == NULL)
		return ;
	(*strtab)[env_size + 1] = NULL;
}

//export OK
//Usage : Premier argument est un pointeur vers les tableau de chaines de caracteres env
// Deuxieme argument un tableau NULL termine de chaines de caracteres NULL terminees listant les variables a ajouter;
int    export(char **env[], char **arg)
{
	char	**env_cpy;
	char	*var_id;
	char	*var;

	if (*arg == NULL)
	{
		env_cpy = build_export_env(env);
		print_export_env(env_cpy);
		ft_free_tab(env_cpy);
	}
	else
	{
		while (*arg)
		{
			var_id = NULL;
			if (check_export_arg(*arg))
				return (1);
			else
				var_id = extract_var_id(*arg);
			if (get_env_var(*env, var_id))
			{
				var = get_env_var(*env, var_id);
				replace_var(env, ft_strdup(*arg), var);
				var = NULL;
			}
			else
				add_str_to_strtab(ft_strdup(*arg), env);
			if (var_id)
				free(var_id);
			arg++;
			// char *tmp = *(arg + 1);
			// free(*arg);
		}
	}
	return (0);
}
