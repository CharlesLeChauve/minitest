#include "minishell.h"

volatile sig_atomic_t	g_interrupted = 0;

typedef struct s_heredoc
{
	char				*hd;
	int					tty_fd;
	int					fd;
	struct sigaction	old_sa;
}						t_heredoc;

void	handle_sigint_h(int sig)
{
	(void)sig;
	g_interrupted = 1;
	write(1, "\n", 1);
}

struct sigaction	*setup_signal_handlers_h(struct sigaction *old_sa)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint_h;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, old_sa) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	return (old_sa);
}

void	restore_signal_handlers(struct sigaction old_sa)
{
	g_interrupted = 0;
	if (sigaction(SIGINT, &old_sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

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

int	read_heredoc(char *limiter, int fd)
{
	t_heredoc	heredoc;
	char		*nl;
	size_t		len;

	heredoc.hd = NULL;
	heredoc.fd = fd;
	len = 0;
	setup_signal_handlers_h(&heredoc.old_sa);
	heredoc.tty_fd = open("/dev/tty", O_RDONLY);
	if (heredoc.tty_fd == -1)
	{
		perror("open");
		return (1);
	}
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

int	get_heredocs(t_cmd_block *cmd_block)
{
	t_list	*current;
	char	*tmp_name;
	int		tmp_fd;
	int		hd_no_1;
	int		hd_no_2;

	current = cmd_block->redirs;
	tmp_name = (char *)malloc(15);
	hd_no_1 = get_hd_no_1();
	hd_no_2 = 0;
	while (current)
	{
		if (((t_token_lst *)(current->content))->type == heredoc)
		{
			ft_bzero(tmp_name, 15);
			ft_sprintf(tmp_name, ".heredoc%d.%d", hd_no_1, hd_no_2);
			tmp_fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (tmp_fd == -1)
			{
				perror("open");
				free(tmp_name);
				return (-1);
			}
			if (read_heredoc(((t_token_lst *)(current->content))->text, \
				tmp_fd) == 2)
			{
				close(tmp_fd);
				free(tmp_name);
				return (1);
			}
			close(tmp_fd);
			((t_token_lst *)(current->content))->type = redir_in;
			free(((t_token_lst *)(current->content))->text);
			((t_token_lst *)(current->content))->text = ft_strdup(tmp_name);
			hd_no_2++;
		}
		current = current->next;
	}
	free(tmp_name);
	return (0);
}
