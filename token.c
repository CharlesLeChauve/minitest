#include "minishell.h"

t_token_lst	*token_new(t_token_type type, void *content)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = type;
	if (content == NULL)
	{
		token->text = NULL;
		return (token);
	}
	else
		token->text = ft_strdup((const char *)content);
	return (token);
}
