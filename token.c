#include "minishell.h"

t_token_lst	*token_new(t_token_type type, void *content)
{
	t_token_lst *token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = type;
	if (content == NULL)
		return (token);
	else
		token->text = ft_strdup((const char *)content);
	return(token);
}

// void	tokenaddback(t_token_lst **lst, t_token_lst *node)
// {
// 	t_token_lst	*act;

// 	if (!node)
// 		return ;
// 	if (!*lst)
// 	{
// 		*lst = node;
// 		return ;
// 	}
// 	act = *lst;
// 	while (act->next)
// 		act = act->next;
// 	act->next = node;
// }

// t_token_lst *token_last(t_token_lst *token_lst)
// {
// 	t_token_lst *node;

// 	node = token_lst;
// 	if (node == NULL)
// 		return (NULL);
// 	while (node->next)
// 		node = node->next;
// 	return (node);
// }
/*
void	treat_token(char *token, t_token_lst **token_lst)
{
	t_token_lst	*last;

	last = token_last(*token_lst);
	if (token[0] == '>' || token[0] == '<')
		tokenaddback(token_lst, token_new(redir, (void *)token));
	else if (token[0] == '|' && token[1] == 0)
		tokenaddback(token_lst, token_new(pipe_op, (void *)token));
	else if (ft_isalpha(token[0]) && last && last->type == redir)
		tokenaddback(token_lst, token_new(file_path, (void *)token));
	else if (ft_isalnum(token[0]) && last && (last->type == command || last->type == option || last->type == argument))
		tokenaddback(token_lst, token_new(argument, (void *)token));
	else if (token[0] == '-' && last &&(last->type == command || last->type == argument || last->type == option))
		tokenaddback(token_lst, token_new(option, (void *)token));
	else if (ft_isvalidchar(token[0]) && (!last || (last && last->type != command)))
		tokenaddback(token_lst, token_new(command, (void *)token));
}*/

void print_token_type(t_token_lst *token){
    switch (token->type) {
        case command:
            printf("Type: command\t");
            break;
        // case argument:
        //     printf("Type: argument\t");
        //     break;
        // case option:
        //     printf("Type: option\t");
        //     break;
        case redir_in:
            printf("Type: redir in\t");
            break;
		case redir_out:
            printf("Type: redir out\t");
            break;
		case redir_app:
            printf("Type: redir app\t");
            break;
		case heredoc:
            printf("Type: heredoc\t");
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
		case eol:
            printf("Type: eol\t");
            break;
        default:
            printf("Unknown type\t");
            break;
    }
	if (token->type == command || token->type == redir_in || token->type == redir_out || token->type == redir_app || token->type == heredoc)
	    printf("Content : [%s]\n", token->text);
}
