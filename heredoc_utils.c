/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:37:48 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 10:37:51 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_interruption(char *nl, t_heredoc *heredoc)
{
	free(nl);
	close(heredoc->tty_fd);
	free(heredoc->hd);
	g_interrupted = 0;
	restore_signal_handlers(heredoc->old_sa);
	return (2);
}

int	handle_end_of_file(t_heredoc *heredoc)
{
	ft_putstr_fd("Error : End Of File before finding here_doc LIMITER\n", 2);
	close(heredoc->tty_fd);
	ft_putstr_fd(heredoc->hd, heredoc->fd);
	free(heredoc->hd);
	restore_signal_handlers(heredoc->old_sa);
	return (1);
}

int	handle_match_limiter(char *nl, t_heredoc *heredoc, char *limiter)
{
	(void)limiter;
	free(nl);
	close(heredoc->tty_fd);
	ft_putstr_fd(heredoc->hd, heredoc->fd);
	free(heredoc->hd);
	restore_signal_handlers(heredoc->old_sa);
	return (0);
}

int	open_tty(int *fd)
{
	*fd = open("/dev/tty", O_RDONLY);
	if (*fd == -1)
	{
		perror("open");
		return (1);
	}
	return (0);
}
