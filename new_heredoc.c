#include "minishell.h"

volatile sig_atomic_t	g_interrupted = 0;

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

int	read_heredoc(char *limiter, int fd)
{
	char	*nl;
	int		tty_fd;
	char	*hd;
	struct sigaction	old_sa;
	size_t	len;

	hd = NULL;
	len = 0;
	setup_signal_handlers_h(&old_sa);
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
	{
		perror("open");
		return (1);
	}
	while (1)
	{
		nl = get_next_line(tty_fd);
		if (g_interrupted)
		{
			free(nl);
			close(tty_fd);
			free(hd);
			g_interrupted = 0;
			restore_signal_handlers(old_sa);
			return (2);
		}
		if (nl == NULL)
		{
			ft_putstr_fd("Error : End Of File before finding here_doc LIMITER\n", 2);
			close(tty_fd);
			ft_putstr_fd(hd, fd);
			free(hd);
			restore_signal_handlers(old_sa);
			return (1);
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter)) && nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			close(tty_fd);
			ft_putstr_fd(hd, fd);
			free(hd);
			restore_signal_handlers(old_sa);
			return (0);
		}
		ft_strappend(&hd, nl, &len);
		free(nl);
	}
	restore_signal_handlers(old_sa);
	return (0);
}

int get_hd_no_1(void)
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
				return (-1);
			}
			if (read_heredoc(((t_token_lst *)(current->content))->text, tmp_fd) == 2)
			{
				close(tmp_fd);
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
	return (0);
}
