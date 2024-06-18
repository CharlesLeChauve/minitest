#include "minishell.h"

int	g_state = 0;

void	handle_sigint_h(int sig)
{
	(void)sig;
	g_state = 13;
}

struct sigaction	*setup_signal_handlers_h(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_old_int;

	sa_int.sa_handler = handle_sigint_h;
	sigemptyset(&sa_int.sa_mask);
	if (sigaction(SIGINT, &sa_int, &sa_old_int) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	// if (sigaction(SIGINT, &sa_old_int, NULL) == -1)
	// {
	// 	perror("sigaction");
	// 	exit(EXIT_FAILURE);
	// }
	return (NULL);
}

int	read_heredoc(char *limiter, int fd)
{
	char	*nl;
	int		tty_fd;
	char	*hd;
	size_t	len;

	len = 0;
	hd = NULL;
	setup_signal_handlers();
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
	{
		perror("open");
		return (1);
	}
	while (1)
	{
		nl = get_next_line(tty_fd);
		/* if (la globale)
			return (2); */
		if (nl == NULL)
		{
			ft_putstr_fd(
				"Error : End Of File before finding here_doc LIMITER\n", 2);
			close(tty_fd);
			ft_putstr_fd(hd, fd);
			free(hd);
			return (1);
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter))
			&& nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			close(tty_fd);
			return(0);
		}
		ft_strappend(&hd, nl, &len);
	}
	ft_putstr_fd(hd, fd);
	free(hd);
	return (0);
}

int	get_hd_no_1(void)
{
	int		hd;
	char	path[15];

	hd = 0;
	while (1)
	{
		ft_bzero(path, 15);
		ft_sprintf(path, ".heredoc%d.0", hd);
		if (access(path, F_OK) == -1)
			break;
		hd++;
	}
	return (hd);
}

int	get_heredocs(t_cmd_block *cmd_block)
{
	t_list	*current;
	char	tmp_name[15];
	int		tmp_fd;
	int		hd_no_1;
	int		hd_no_2;

	hd_no_1 = get_hd_no_1();
	hd_no_2 = 0;
	current = cmd_block->redirs;
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
			if (((t_token_lst *)(current->content))->text[0] ==  read_heredoc(((t_token_lst *)(current->content))->text, tmp_fd))
				return (1);
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
