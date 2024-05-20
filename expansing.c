#include "minishell.h"

t_cmd_block *init_cmd_block(void)
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

void	set_command_quotes_state(t_tkn_info *tkn_info)
{
	int same;

	same = 1;
	if (tkn_info->quote_level == 0 && (*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\''))
	{
			tkn_info->quote_level++;
			tkn_info->first_quote = *tkn_info->curr_char;
			if (*tkn_info->curr_char == '"')
				tkn_info->state = dquote;
			else
				tkn_info->state = quote;
			tkn_info->curr_char++;
			while ((*tkn_info->curr_char == '\"' || *tkn_info->curr_char == '\'') && same)
			{
				same = same_quote(tkn_info);
				tkn_info->curr_char++;
				tkn_info->quote_level++;
			}
	}
	else if (same_quote(tkn_info))
	{
		if (tkn_info->quote_level == 1)
		{
			tkn_info->state = reg;
			tkn_info->quote_level--;
		}
		else
			tkn_info->quote_level--;
		tkn_info->curr_char++;
	}
}

void parse_command_option(char *token, t_cmd_block *block)
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
	{
		block->commande = ft_strndup(token, ptr - token);
		printf("Command set: %s\n", block->commande);
	}
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
				printf("Option added: %s\n", sub_token);
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
				printf("Argument added: %s\n", sub_token);
			}
			free(sub_token);
		}
	}
}

void fill_cmd_block(t_cmd_block *block, t_dlist *tokens)
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
	// Print the final cmd_block structure for debugging
	printf("Command: %s\n", block->commande);
	if (block->option)
	{
		opt = block->option;
		while (opt)
		{
			printf("  - %s\n", (char *)opt->content);
			opt = opt->next;
		}
	}
	if (block->arg)
	{
		arg = block->arg;
		while (arg)
		{
			printf("  - %s\n", (char *)arg->content);
			arg = arg->next;
		}
	}
	if (block->redir_in)
	{
		redir = block->redir_in;
		while (redir)
		{
			printf("Redirections in:\n");
			printf("  - %s\n", ((t_token_lst *)redir->content)->text);
			redir = redir->next;
		}
	}
	if (block->redir_out)
	{
		printf("Redirections out:\n");
		redir = block->redir_out;
		while (redir)
		{
			printf("  - %s\n", ((t_token_lst *)redir->content)->text);
			redir = redir->next;
		}
	}
}

