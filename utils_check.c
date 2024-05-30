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
			if (!prev)
				return (fprintf(stderr, "tash: syntax error near unexpected token `%s'\n", token->text), 0);
			// if (!prev || !current->next \
			// 	|| (!is_command(((t_token_lst *)prev->content)->type) \
			// 	&& !is_redir(((t_token_lst *)prev->content)->type)) \
			// 	|| !is_command(((t_token_lst *)current->next->content)->type)) {
			// 	return (fprintf(stderr, "Error: Operator '%s' not between two commands\n", token->text), 0);
			// }
		if (current->next \
			&& (is_operator(((t_token_lst *)current->next->content)->type) \
			|| ((t_token_lst *)current->next->content)->type == pipe_op))
		{
			if (((t_token_lst *)current->next->content)->type == eol)
				{
					char	*input = get_next_line(0);
					if (!input)
						return (1);
					current->next->next->content = token_new(command, input);
					current->next->next->next->content = token_new(eol, NULL);
				}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

