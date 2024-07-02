#include "minishell.h"

void	read_subshell(t_tkn_info *tkn_info, char **buffer, size_t *len, \
int *par_lvl)
{
	while (*tkn_info->curr_char)
	{
		if (*tkn_info->curr_char == '(')
			(*par_lvl)++;
		if (*tkn_info->curr_char == ')' && *par_lvl)
			(*par_lvl)--;
		else if (*tkn_info->curr_char == ')')
		{
			tkn_info->curr_char++;
			break ;
		}
		ft_add_char_to_buffer(buffer, *tkn_info->curr_char, len);
		tkn_info->curr_char++;
	}
	if (*buffer)
		(*buffer)[ft_strlen(*buffer)] = '\0';
}

t_token_lst	*subshell_token(t_tkn_info *tkn_info)
{
	t_token_lst	*token;
	size_t		len;
	char		*buffer;
	int			par_lvl;

	par_lvl = 0;
	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = subshell;
	len = 0;
	buffer = NULL;
	read_subshell(tkn_info, &buffer, &len, &par_lvl);
	token->text = buffer;
	return (token);
}

t_token_lst	*handle_paren(t_tkn_info *tkn_info)
{
	tkn_info->curr_char++;
	return (subshell_token(tkn_info));
}
