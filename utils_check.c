#include "minishell.h"

int	is_redir(t_token_type type)
{
	if (type == redir_app || type == redir_in || type == redir_out || \
		type == heredoc)
		return (1);
	return (0);
}

int	is_command(t_token_type type)
{
	return (type == command);
}

int	is_operator(t_token_type type)
{
	return (type == and_op || type == or_op || type == pipe_op);
}

int	verify_tokens(t_dlist *tokens)
{
	t_dlist			*current;
	t_dlist			*prev;
	t_token_lst		*token;

	current = tokens;
	prev = NULL;
	if (!tokens)
		return (fprintf(stderr, "Error: No tokens provided\n"), 0);
	while (current != NULL)
	{
		token = (t_token_lst *)current->content;
		if (is_operator(token->type))
			if (!prev || !current->next || (!is_command(((t_token_lst *)prev->content)->type) && !is_redir(((t_token_lst *)prev->content)->type)) || !is_command(((t_token_lst *)current->next->content)->type))
				return (fprintf(stderr, "Error: Operator '%s' not between two commands\n", token->text), 0);
		if (is_redir(token->type))
			if (!current->next)
				return (fprintf(stderr, "Error: Redirection '%s' not followed by a command\n", token->text), 0);
		if (current->next && is_operator(((t_token_lst *)current->next->content)->type) && ((t_token_lst *)current->next->content)->type == pipe_op)
			if (!current->next->next || !is_command(((t_token_lst *)current->next->next->content)->type))
				return (fprintf(stderr, "Error: Redirection '%s' not followed by a command\n", token->text), 0);
		prev = current;
		current = current->next;
	}
	return (1);
}
