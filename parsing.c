#include "minishell.h"

void	handle_sigint(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

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

void	swap_char(char *c1, char *c2)
{
	char	tmp;

	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

void	revstr(char *str)
{
	unsigned int	size;
	unsigned int	i;

	size = ft_strlen(str);
	i = -1;
	while (++i < --size)
		swap_char(&str[i], &str[size]);
}

void	treat_token(char *token, t_token **token_lst)
{
	if (token[0] == '>' || token[0] == '>')
		tokenaddback(token_lst, token_new(redirection, (void *)token));
	else if (token[0] == '|' && token[1] == 0)
		tokenaddback(token_lst, token_new(pipe_op, NULL));
	else if (ft_isalpha(token[0]) && (token_last(*token_lst))->type != command)
		tokenaddback(token_lst, token_new(command, (void *)token));
	else if (token[0] == '-' && ((token_last(*token_lst))->type == command))
		tokenaddback(token_lst, token_new(option, (void *)token));
}

void	parse_input(char *input, t_token **token_lst)
{
	char	**input_tab;
	int	i;

	input_tab = ft_split(input, ' ');
	i = 0;
	while (input_tab[i])
	{
		treat_token(input_tab[i], token_lst);
		i++;
	}
}

void print_token_type(t_token *token) {
    switch (token->type) {
        case command:
            printf("Type: command\n");
            break;
        case argument:
            printf("Type: argument\n");
            break;
        case option:
            printf("Type: option\n");
            break;
        case redirection:
            printf("Type: redirection\n");
            break;
        case pipe_op:
            printf("Type: pipe_op\n");
            break;
        case fd:
            printf("Type: fd\n");
            break;
        default:
            printf("Unknown type\n");
            break;
    }
}

int	main(void)
{
	char			*input;
	struct sigaction	sa;
	t_token			*token_lst;
	t_token			*token_node;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		input = readline("Maxishell > ");
		if (!input)
			break;
		if (input && *input)
		{
			add_history(input);
			parse_input(input, &token_lst);
			revstr(input);
			printf("%s\n", input);
			token_node = token_lst;
			while (token_node)
			{
				print_token_type(token_node);
				token_node = token_node->next;
			}
		}
		free(input);
	}
}
