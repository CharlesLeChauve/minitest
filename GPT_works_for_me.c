#include "minishell.h"

void	create_exec_tab(t_cmd_block *cmd_block)
{
	int		i;
	t_list	*current;

	expand_wildcards_in_block(cmd_block);
	i = ft_lstsize(cmd_block->arg);
	cmd_block->exec_tab = (char **)malloc((i + 2) * sizeof(char *));
	cmd_block->exec_tab[i + 1] = NULL;
	if (!cmd_block->exec_tab) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	current = cmd_block->arg;
	while (current)
	{
		cmd_block->exec_tab[i] = ft_strdup((char *)current->content);
		current = current->next;
		i++;
	}
	if (current && cmd_block->exec_tab[0])
		cmd_block->command = ft_strdup(cmd_block->exec_tab[0]);
	cmd_block->exec_tab[i] = NULL;
}
