#include "minishell.h"

t_cmd_block	*init_cmd_block(void)
{
	t_cmd_block	*block;

	block = malloc(sizeof(t_cmd_block));
	if (!block)
		return (NULL);
	block->exec_tab = NULL;
	block->commande = NULL;
	block->option = NULL;
	block->arg = NULL;
	block->redir_in = NULL;
	block->redir_out = NULL;
	return (block);
}

void	parse_command_option(char *token, t_cmd_block *block)
{
	char	*ptr;
	char	*start;
	char	*sub_token;
	t_list	*new_opt;
	t_list	*new_arg;

	ptr = token;
	while (*ptr && !ft_isspace(*ptr))
		ptr++;
	if (ptr > token)
		block->commande = ft_strndup(token, ptr - token);
	while (*ptr)
	{
		while (ft_isspace(*ptr))
			ptr++;
		start = ptr;
		while (*ptr && !ft_isspace(*ptr))
			ptr++;
		if (ptr > start)
		{
			sub_token = ft_strndup(start, ptr - start);
			if (sub_token[0] == '-')
			{
				new_opt = ft_lstnew(sub_token);
				ft_lstadd_back(&(block->option), new_opt);
				continue ;
			}
			if (sub_token[0] == '>' || sub_token[0] == '<')
			{
				new_arg = ft_lstnew(redir_token(sub_token));
				if (sub_token[0] == '>')
					ft_lstadd_back(&(block->redir_out), new_arg);
				if (sub_token[0] == '<')
					ft_lstadd_back(&(block->redir_in), new_arg);
			}
			else
			{
				new_arg = ft_lstnew(sub_token);
				ft_lstadd_back(&(block->arg), new_arg);
			}
			// free(sub_token);
		}
	}
}

void	fill_cmd_block(t_cmd_block *block, t_dlist *tokens)
{
	t_dlist		*current = tokens;
	t_list		*new_redir;
	t_list		*redir;
	t_token_lst	*token;
	t_list		*opt;
	t_list		*arg;
	char		*token_text;

	while (current)
	{
		token = (t_token_lst *)(current->content);
		token_text = token->text;
		if (token_text == NULL)
		{
			current = current->next;
			continue;
		}
		if (token->type == redir_in || token->type == redir_out || token->type == redir_app || token->type == heredoc)
		{
			new_redir = ft_lstnew(token_text);
			if (token->type == redir_in || token->type == heredoc)
			{
				ft_lstadd_back(&(block->redir_in), new_redir);
				printf("Redirection in added: %s\n", token_text);
			}
			else
			{
				ft_lstadd_back(&(block->redir_out), new_redir);
				printf("Redirection out added: %s\n", token_text);
			}
		}
		else
			parse_command_option(token_text, block);
		current = current->next;
	}
}

void	print_cmd_block(t_cmd_block *cmd_block)
{
		t_list		*opt;
		t_list		*arg;
		t_list		*redir;

		if (cmd_block->option)
		{
			opt = cmd_block->option;
			while (opt)
			{
				printf("  Option: %s\n", (char *)opt->content);
				opt = opt->next;
			}
		}
		if (cmd_block->arg)
		{
			arg = cmd_block->arg;
			while (arg)
			{
				printf("  Argument: %s\n", (char *)arg->content);
				arg = arg->next;
			}
		}
		if (cmd_block->redir_in)
		{
			redir = cmd_block->redir_in;
			while (redir)
			{
				printf("Redirections in:\n");
				printf("  %s\n", ((t_token_lst *)redir->content)->text);
				redir = redir->next;
			}
		}
		if (cmd_block->redir_out)
		{
			redir = cmd_block->redir_out;
			while (redir)
			{
				printf("Redirections out:\n");
				printf("  %s\n", ((t_token_lst *)redir->content)->text);
				redir = redir->next;
			}
		}
}

void	expand_ast(t_ast_node *node)
{
	t_cmd_block	*cmd_block;


	if (node == NULL)
		return ;
	//Le commande block cree a l'air nickel mais il n'est jamais affecte a rien, on le erd apres cette fonction
	//est-ce qu'on ajoute un pointeur a la structure ast_node ?
	//
	if (node->type == command)
	{
		//cmd_block = init_cmd_block();
		node->cmd_block = init_cmd_block();
		fill_cmd_block(node->cmd_block, node->tokens);
		// node->cmd_block = cmd_block;
		// print_cmd_block(node->cmd_block);
	}
	expand_ast(node->left);
	expand_ast(node->right);
}

void	clear_cmd_block(t_cmd_block *block)
{
	ft_lstclear(&(block->option), free);
	ft_lstclear(&(block->arg), free);
	ft_lstclear(&(block->redir_in), free);
	ft_lstclear(&(block->redir_out), free);
	free(block->commande);
	free(block);
}
