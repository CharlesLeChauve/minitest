#include "minishell.h"



void	set_quotes_state(t_tkn_info *tkn_info)
{
	int same = 1;
	int	state = tkn_info->state;
	int quotelevel = tkn_info->quote_level;
	char	first;

	if (tkn_info->curr_char == '"' || tkn_info->curr_char == '\'')
		first = tkn_info->curr_char;
	while ((tkn_info->curr_char == '\"' || tkn_info->curr_char == '\'') && same)
	{
		if (tkn_info->curr_char == '\0' || tkn_info->curr_char == '\n' || tkn_info->curr_char == ' ')
			break ;
		tkn_info->curr_char++;
		state = (tkn_info->curr_char == '\"') ? dquote : quote;
		quotelevel++;
		if (tkn_info->curr_char != first)
			same = 0;
	}
	if (same_quote(tkn_info))
	{
		if (quotelevel == 1)
		{
			state = reg;
			quotelevel--;
		}
		else
			quotelevel--;
	}
	if (quotelevel != 0)
		fprintf(stderr, "Error: quotes not closed\n");
}

int	same_quote(t_tkn_info *tkn_info)
{
	if (tkn_info->state == dquote && tkn_info->quote_level == 2)
		return (1);
	else if (tkn_info->state == quote && tkn_info->quote_level == 1)
		return (1);
	return (0);
}
