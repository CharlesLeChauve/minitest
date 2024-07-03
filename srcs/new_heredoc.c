/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:37:52 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:20 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	read_heredoc(char *limiter, int fd)
{
	t_heredoc	heredoc;
	char		*nl;
	size_t		len;

	heredoc.hd = NULL;
	heredoc.fd = fd;
	len = 0;
	setup_signal_handlers_h(&heredoc.old_sa);
	if (open_tty(&heredoc.tty_fd))
		return (1);
	while (1)
	{
		nl = get_next_line(heredoc.tty_fd);
		if (g_interrupted)
			return (handle_interruption(nl, &heredoc));
		if (nl == NULL)
			return (handle_end_of_file(&heredoc));
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter)) \
			&& nl[ft_strlen(limiter)] == '\n')
			return (handle_match_limiter(nl, &heredoc, limiter));
		ft_strappend(&heredoc.hd, nl, &len);
		free(nl);
	}
	restore_signal_handlers(heredoc.old_sa);
	return (0);
}

int	get_hd_no_1(void)
{
	int		hd;
	char	*path;

	hd = 0;
	path = (char *)malloc(15);
	while (1)
	{
		ft_bzero(path, 15);
		ft_sprintf(path, ".heredoc%d.0", hd);
		if (access(path, F_OK) == -1)
			break ;
		hd++;
	}
	free(path);
	return (hd);
}

int	handle_heredoc(t_token_lst *token, int hd_no_1, int *hd_no_2)
{
	char	tmp_name[128];
	int		tmp_fd;

	ft_bzero(tmp_name, 128);
	ft_sprintf(tmp_name, ".heredoc%d.%d", hd_no_1, *hd_no_2);
	tmp_fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (read_heredoc(token->text, tmp_fd) == 2)
	{
		close(tmp_fd);
		return (1);
	}
	close(tmp_fd);
	token->type = redir_in;
	free(token->text);
	token->text = ft_strdup(tmp_name);
	(*hd_no_2)++;
	return (0);
}

int	get_heredocs(t_cmd_block *cmd_block)
{
	t_list	*current;
	int		hd_no_1;
	int		hd_no_2;
	int		ret;

	current = cmd_block->redirs;
	hd_no_1 = get_hd_no_1();
	hd_no_2 = 0;
	while (current)
	{
		if (((t_token_lst *)(current->content))->type == heredoc)
		{
			ret = handle_heredoc(((t_token_lst *)(current->content)), \
				hd_no_1, &hd_no_2);
			if (ret != 0)
				return (ret);
		}
		current = current->next;
	}
	return (0);
}
