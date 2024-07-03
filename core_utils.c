#include "minishell.h"

void	incr_shlvl(char **var)
{
	int		i;
	int		lvl;
	char	*str_lvl;

	i = 0;
	while ((*var)[i])
		i++;
	i--;
	while (ft_isdigit((*var)[i]))
		i--;
	i++;
	lvl = ft_atoi(&(*var)[i]);
	lvl++;
	str_lvl = ft_itoa(lvl);
	while ((*var)[i])
		(*var)[i++] = '\0';
	*var = ft_strjoin_free(*var, str_lvl, 2);
}

char	**set_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (envp[i] == get_env_var(envp, "SHLVL"))
			incr_shlvl(&env[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*build_prompt(void)
{
	char	*pwd;
	char	*prompt;

	pwd = get_cwd();
	if (pwd == NULL)
		return ("Minishell_project");
	prompt = ft_strdup(TASH_PROMPT_S);
	prompt = ft_strjoin_free(prompt, pwd, 2);
	prompt = ft_strjoin_free(prompt, TASH_PROMPT_E, 0);
	return (prompt);
}

char	*prompted_readline(void)
{
	char	*input;
	char *prompt;

	prompt = build_prompt();
	input = readline(prompt);
	free(prompt);
	return (input);
}
