#include "minishell.h"

void builtin_echo(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		newline = 0; // Ne pas imprimer de nouvelle ligne à la fin
		i = 2;       // Commencer après l'option -n
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1); // Espace entre les mots
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

void builtin_cd(char **args)
{
	if (args[1] == NULL || chdir(args[1]) != 0)
		perror("cd");
}

void builtin_pwd(void)
{
	char *cwd;

	cwd = getcwd(cwd, sizeof(cwd));
	if (cwd != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void builtin_export(char **args)
{
	char	*key;
	char	*value;
	int	i;
	extern char **env;

	i = 0;
	if (args[1] == NULL)
	{
		while (env[i])
		{
			printf("%s\n", env[i]);
			i++;
		}
	}
	else
	{
		key = strtok(args[1], "=");
		value = strtok(NULL, "=");
		if (value)
		{
			setenv(key, value, 1); // pas autorise mais t'as capte l'idee bg
		}
	}
}

void builtin_env(void)
{
	extern char		**env;
	int				i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}


void builtin_exit(char **args)
{
	exit(0);
}

