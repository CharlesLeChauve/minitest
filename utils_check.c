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

int	no_text(char *text)
{
	if (!text)
		return (1);
	while (*text)
	{
		if (*text != '(' && *text != ')' && !ft_isspace(*text))
			return (0);
		text++;
	}
	return (1);
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
		{
			if (!prev)
				return (fprintf(stderr, "tash: syntax error near unexpected token `%s'\n", token->text), 0);
		}
		else if (token->type == subshell && (!token->text || no_text(token->text)))
		{
			return (fprintf(stderr, "tash: syntax error near unexpected token `)'\n"), 0);
		}
		else if (is_redir(token->type))
		{
			if (!current->next)
				return (fprintf(stderr, "Error: Redirection '%s' not followed by a command\n", token->text), 0);
			// if (is_redir(((t_token_lst *)current->next->content)->type))
			// 	return (fprintf(stderr, "Error: Invalid redirection sequence '%s%s'\n",
					// token->text, ((t_token_lst *)current->next->content)->text), 0);
		}
		else if (current->next \
			&& is_operator(((t_token_lst *)current->next->content)->type) \
			&& ((t_token_lst *)current->next->content)->type == pipe_op) {
			if (!current->next->next \
				|| !is_command(((t_token_lst *)current->next->next->content)->type))
				return (fprintf(stderr, "Error: Redirection '%s' not followed by a command\n", token->text), -1);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
/* 
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
					// char	*input = get_next_line(0);
					// if (!input)
					// 	return (1);
					// current->next->next->content = token_new(command, input);
					// current->next->next->next->content = token_new(eol, NULL);
					return (-1);
				}
		}
		prev = current;
		current = current->next;
	}
	return (1);
} */

