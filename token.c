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

// void	print_token_type(t_token_lst *token)
// {
// 	switch (token->type)
// 	{
// 		case command:
// 			printf("Type: command\t");
// 			break ;
// 		case redir_in:
// 			printf("Type: redir in\t");
// 			break ;
// 		case redir_out:
// 			printf("Type: redir out\t");
// 			break ;
// 		case redir_app:
// 			printf("Type: redir app\t");
// 			break ;
// 		case heredoc:
// 			printf("Type: heredoc\t");
// 			break ;
// 		case pipe_op:
// 			printf("Type: pipe_op\t");
// 			break ;
// 		case fd:
// 			printf("Type: fd\t");
// 			break ;
// 		case file_path:
// 			printf("Type: file_path\t");
// 			break ;
// 		case eol:
// 			printf("Type: eol\t\n");
// 			break ;
// 		case subshell:
// 			printf("Type: subshell\t\n");
// 			break ;
// 		case or_op:
// 			printf("Type: or_op\t\n");
// 			break ;
// 		case and_op:
// 			printf("Type: and_op\t\n");
// 			break ;
// 		default :
// 			printf("Unknown type\t");
// 				break;
// 	}
// 	if ((token->type == command || token->type == redir_in || token->type == redir_out ||
// 			token->type == redir_app || token->type == heredoc || token->type == subshell) && token->text != NULL)
// 		printf("Content : [%s]\n", token->text);
// }
