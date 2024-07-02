#include "minishell.h"

int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || *str == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_with_message(t_shell *shl, int exit_code,
const char *message, int fd)
{
	if (message)
		ft_putstr_fd((char *)message, fd);
	ft_free_tab(shl->env);
	clean_shell_instance(shl);
	exit(exit_code);
}

int	check_numeric_argument(char *arg)
{
	if (arg[0] == '+' || arg[0] == '-')
	{
		return (ft_is_numeric(arg + 1));
	}
	return (ft_is_numeric(arg));
}

int	handle_arguments(char **args, t_shell *shl)
{
	if (args[1] == NULL)
		exit_with_message(shl, 0, "exit\n", 2);
	if (!check_numeric_argument(args[1]))
		exit_with_message(shl, 2, "exit: numeric argument required\n", 2);
	return (ft_atoi(args[1]));
}

int	check_additional_arguments(char **args)
{
	if (args[2] != NULL)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	ft_exit(char **args, t_shell *shl)
{
	int	exit_code;

	exit_code = handle_arguments(args, shl);
	if (check_additional_arguments(args))
		return (1);
	exit_with_message(shl, exit_code, "exit\n", 1);
	return (0);
}
