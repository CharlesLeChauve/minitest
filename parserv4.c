#include "minishell.h"

int	ft_isshelloperator(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&')
		return (1);
	return (0);
}

void ft_add_char_to_buffer(char **buffer, char c, size_t *len, t_tkn_info *tkn_info)
{
    char *new_buffer;
    if ((tkn_info->state == quote && c == '\'' && *len > 0 && (*buffer)[*len - 1] != '\\') ||
        (tkn_info->state == dquote && c == '"' && *len > 0 && (*buffer)[*len - 1] != '\\'))
    {
        tkn_info->state = reg;
        return;
    }
    new_buffer = malloc(*len + 2);
    if (!new_buffer)
    {
        ft_printf("Error: Allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if (*buffer) ft_memcpy(new_buffer, *buffer, *len);
    new_buffer[*len] = c;
    (*len)++;
    new_buffer[*len] = '\0';
    free(*buffer);
    *buffer = new_buffer;
}



void set_quotes_state(t_tkn_info *tkn_info)
{
    if (*tkn_info->curr_char == '\'' && (tkn_info->curr_char == tkn_info->input || *(tkn_info->curr_char - 1) != '\\'))
    {
        if (tkn_info->state == reg || tkn_info->state == dquote) // Si le précédent état était régulier ou guillemets doubles, passe à l'état des guillemets simples
            tkn_info->state = quote;
        else if (tkn_info->state == quote && *(tkn_info->curr_char + 1) == '\'') // Si le précédent état était guillemets simples et le prochain caractère est une apostrophe, reste dans l'état des guillemets simples
            tkn_info->curr_char++; // Avancez pour ignorer l'apostrophe suivante
        else // Sinon, retourne à l'état régulier
            tkn_info->state = reg;
        tkn_info->curr_char++; // Avancez uniquement après avoir changé l'état
    }
    else if (*tkn_info->curr_char == '"' && (tkn_info->curr_char == tkn_info->input || *(tkn_info->curr_char - 1) != '\\'))
    {
        if (tkn_info->state == reg || tkn_info->state == quote) // Si le précédent état était régulier ou guillemets simples, passe à l'état des guillemets doubles
            tkn_info->state = dquote;
        else if (tkn_info->state == dquote && *(tkn_info->curr_char + 1) == '"') // Si le précédent état était guillemets doubles et le prochain caractère est un guillemet, reste dans l'état des guillemets doubles
            tkn_info->curr_char++; // Avancez pour ignorer le guillemet suivant
        else // Sinon, retourne à l'état régulier
            tkn_info->state = reg;
        tkn_info->curr_char++; // Avancez uniquement après avoir changé l'état
    }
}


void	space_quotes(t_tkn_info *tkn_info)
{
	if (tkn_info->state == reg)
		while (ft_isspace(*(tkn_info->curr_char)))
			tkn_info->curr_char++;
	set_quotes_state(tkn_info);
}

int	break_token(t_tkn_info *tkn_info, int is_redir)
{
	t_sm	state;

	state = tkn_info->state;
	if (state == reg && (ft_isshelloperator(*tkn_info->curr_char)))
		return (1);

	else if ((state == quote && *tkn_info->curr_char == '\'') || (state == dquote && *tkn_info->curr_char == '"') && is_redir)
		return (1);
	else
		return (0);
}

void set_token_text(t_tkn_info *tkn_info, t_token_lst *token)
{
    size_t len = 0;
    char *buffer = NULL;
    while (*tkn_info->curr_char)
    {
        if (break_token(tkn_info, 0))
            break;
        // if (ft_isspace(*tkn_info->curr_char))
        // {
		// 	space_quotes(tkn_info);
        //     char *peek = tkn_info->curr_char + 1;
        //     while (ft_isspace(*peek))
		// 		peek++;
        //     if ((ft_isshelloperator(*peek) || *peek == '|' || *peek == '\0' || *peek == '\n') && tkn_info->state == reg)
        //         break;
        // }
		if (*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\'')
		{
			set_quotes_state(tkn_info);

			continue;
		}
        ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len, tkn_info);
        tkn_info->curr_char++;
    }
    while (len > 0 && ft_isspace(buffer[len - 1]))
    {
        buffer[len - 1] = '\0';
        len--;
    }
    token->text = buffer;  // Assurez-vous que buffer n'est pas NULL avant de l'utiliser
}


t_token_lst	*redir_token(t_tkn_info *tkn_info)
{
	t_token_lst		*token;
	char			*buffer;
	size_t			len;

	buffer = NULL;
	len = 0;
	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!token)
	{
		perror("Allocation Failed");
		exit(EXIT_FAILURE);
	}
	if (*tkn_info->curr_char == '>')
	{
		tkn_info->curr_char++;
		if (*tkn_info->curr_char == '>')
		{
			tkn_info->curr_char++;
			token->type = redir_app;
		}
		else
			token->type = redir_out;
	}
	else if (*tkn_info->curr_char == '<')
	{
		tkn_info->curr_char++;
		if (*tkn_info->curr_char == '<')
		{
			tkn_info->curr_char++;
			token->type = heredoc;
		}
		else
			token->type = redir_in;
	}
	while (*tkn_info->curr_char == ' ')
		tkn_info->curr_char++;
	while (*tkn_info->curr_char && *tkn_info->curr_char != ' ' \
			&& *tkn_info->curr_char != '|')
	{
		if ((*tkn_info->curr_char == '"' || *tkn_info->curr_char == '\''))
		{
			set_quotes_state(tkn_info);
			break ;
		}
		ft_add_char_to_buffer(&buffer, *tkn_info->curr_char, &len, tkn_info);
		tkn_info->curr_char++;
	}
	if (len > 0 && (buffer[len - 1] == '"' || buffer[len - 1] == '\''))
	{
		buffer[len - 1] = '\0';
		len--;
	}
	token->text = buffer;
	return (token);
}

t_token_lst	*cmd_token(t_tkn_info *tkn_info)
{
	t_token_lst *token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	token->type = command;
	set_token_text(tkn_info, token);
	return (token);
}


t_token_lst *next_token(t_tkn_info *tkn_info)
{
    space_quotes(tkn_info);
    if (!*tkn_info->curr_char)  // Plus sécurisé pour les chaînes vides
        return token_new(eol, NULL);
    if (*tkn_info->curr_char == '|') {
        tkn_info->curr_char++;
        if (*tkn_info->curr_char == '|') {
            tkn_info->curr_char++;
            return token_new(or_op, strdup("||"));
        }
        return token_new(pipe_op, strdup("|"));
    } else if (*tkn_info->curr_char == '&' && *(tkn_info->curr_char + 1) == '&') {
        tkn_info->curr_char += 2;
        return token_new(and_op, strdup("&&"));
    }
    if ((*tkn_info->curr_char == '>' || *tkn_info->curr_char == '<') && tkn_info->state == reg)
        return redir_token(tkn_info);

    return cmd_token(tkn_info);
}



t_dlist *tokenize(char *input)
{
    if (!input || !*input) {  // Vérifiez si l'input est NULL ou vide
        return NULL;
    }

    t_dlist *last;
    t_tkn_info tkn_info;

    tkn_info.input = ft_strdup(input);
    tkn_info.curr_char = tkn_info.input;  // Doit pointer sur la copie, pas sur l'original
    tkn_info.token_lst = NULL;
    tkn_info.state = reg;
    ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(next_token(&tkn_info)));
    last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
    while (last && ((t_token_lst *)(last->content))->type != eol)
    {
        ft_dlstadd_back(&tkn_info.token_lst, ft_dlstnew(next_token(&tkn_info)));
        last = (t_dlist *)ft_lstlast((t_list *)tkn_info.token_lst);
    }
    return tkn_info.token_lst;
}

