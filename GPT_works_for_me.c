#include "minishell.h"

void create_exec_tab(t_cmd_block *cmd_block) {
	int i;
	int total_size;
	t_list *current;

	i = ft_lstsize(cmd_block->arg);
	cmd_block->exec_tab = (char **)malloc((i + 2) * sizeof(char *));
	if (!cmd_block->exec_tab) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	cmd_block->exec_tab[i++] = cmd_block->command;
	current = cmd_block->arg;
	while (current) 
	{
		cmd_block->exec_tab[i++] = ft_strdup((char *)current->content);
		current = current->next;
	}
	cmd_block->exec_tab[i] = NULL;
}
