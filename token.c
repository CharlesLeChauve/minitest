#include "minishell.h"

t_token	*token_new(t_token_type type, void *content)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	token->next = NULL;
	token->type = type;
	if (content == NULL)
		return (token);
	if (type == fd)
		token->fd = *(int *)content;
	else
		token->text = ft_strdup((const char *)content);
	return(token);
}

void	tokenaddback(t_token **lst, t_token *node)
{
	t_token	*act;

	if (!*lst)
	{
		*lst = node;
		return ;
	}
	act = *lst;
	while (act->next)
		act = act->next;
	act->next = node;
}

t_token *token_last(t_token *token_lst)
{
	t_token *node;

	node = token_lst;
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}

void	treat_token(char *token, t_token **token_lst)
{
	t_token	*last;

	last = token_last(*token_lst);
	if (token[0] == '>' || token[0] == '<')
		tokenaddback(token_lst, token_new(redirection, (void *)token));
	else if (token[0] == '|' && token[1] == 0)
		tokenaddback(token_lst, token_new(pipe_op, (void *)token));
	else if (ft_isalpha(token[0]) && last && last->type == redirection)
		tokenaddback(token_lst, token_new(file_path, (void *)token));
	else if (ft_isalnum(token[0]) && last && (last->type == command || last->type == option || last->type == argument))
		tokenaddback(token_lst, token_new(argument, (void *)token));
	else if (token[0] == '-' && last &&(last->type == command || last->type == argument || last->type == option))
		tokenaddback(token_lst, token_new(option, (void *)token));
	else if (ft_isvalidchar(token[0]) && (!last || (last && last->type != command)))
		tokenaddback(token_lst, token_new(command, (void *)token));
}

void print_token_type(t_token *token){
    switch (token->type) {
        case command:
            printf("Type: command\t");
            break;
        case argument:
            printf("Type: argument\t");
            break;
        case option:
            printf("Type: option\t");
            break;
        case redirection:
            printf("Type: redir\t");
            break;
        case pipe_op:
            printf("Type: pipe_op\t");
            break;
        case fd:
            printf("Type: fd\t");
            break;
	case file_path:
            printf("Type: file_path\t");
            break;
        default:
            printf("Unknown type\t");
            break;
    }
    printf("Content : [%s]\n", token->text);
}
