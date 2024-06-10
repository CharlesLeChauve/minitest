/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:18:03 by tgibert           #+#    #+#             */
/*   Updated: 2024/06/10 15:15:10 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_h(int sig)
{
	exit(13);
}

void	handle_sigquit_h()
{
	return ;
}

struct sigaction	*setup_signal_handlers_h(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_old_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_h;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, &sa_old_int) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sa_quit.sa_handler = handle_sigquit_h;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void	read_heredoc(char *limiter)
{
	char	*nl;
	int		tty_fd;

	tty_fd = open("/dev/tty", O_RDONLY);
	while (1)
	{
		nl = get_next_line(tty_fd);
		if (nl == NULL)
		{
			ft_putstr_fd(
				"Error : End Of File before finding here_doc LIMITER\n", 2);
			exit(EXIT_FAILURE);
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter))
			&& nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			nl = NULL;
			fflush(tty_fd);
			close(tty_fd);
			exit(0);
		}
		ft_putstr_fd(nl, 1);
		free(nl);
	}
}

int	heredoc_handle(char *limiter, t_std_fd_save save)
{
	int	pipe_fd[2];
	int	pid;
	int	status;

	if (pipe(pipe_fd) == -1)
		exit(-1);
	pid = fork();
	if (pid == 0)
	{
		setup_signal_handlers_h();
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		read_heredoc(limiter);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return (-1);
		}
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 13)
				return (130);
			return (WEXITSTATUS(status));
		}
	}
}
