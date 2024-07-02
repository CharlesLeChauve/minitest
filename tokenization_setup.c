#include "minishell.h"

int	break_token(t_tkn_info *tkn_info)
{
	first_read_quotes(tkn_info);
	if ((tkn_info->state == reg && ft_isshelloperator(*tkn_info->curr_char)))
		return (1);
	else
		return (0);
}

void	set_token_text(t_tkn_info *tkn_info, t_token_lst *token)
{
	size_t		len;
	char		*buffer;

	len = 0;
	buffer = NULL;
	while (*tkn_info->curr_char)
	{
		if (tkn_info->curr_char != tkn_info->input && break_token(tkn_info))
			break ;
		ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len);
		tkn_info->curr_char++;
	}
	while (len > 0 && ft_isspace(buffer[len - 1]))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	token->text = buffer;
}

t_token_lst	*cmd_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = command;
	set_token_text(tkn_info, token);
	return (token);
}

t_token_lst	*handle_pipe(t_tkn_info *tkn_info)
{
	tkn_info->curr_char++;
	if (*tkn_info->curr_char == '|')
	{
		tkn_info->curr_char++;
		return (token_new(or_op, "||"));
	}
	return (token_new(pipe_op, "|"));
}

t_token_lst	*handle_ampersand(t_tkn_info *tkn_info)
{
	tkn_info->curr_char++;
	if (*tkn_info->curr_char == '&')
	{
		tkn_info->curr_char++;
		return (token_new(and_op, "&&"));
	}
	return (NULL);
}
