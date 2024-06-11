#include "minishell.h"

void	handle_sigint_h(int sig)
{
	exit(13);
}

void	handle_sigquit_h(int sig)
{
	exit(1) ;
}

struct sigaction	*setup_signal_handlers_h(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_old_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_h;
	sigemptyset(&sa_int.sa_mask);
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

void	read_heredoc(char *limiter, int fd)
{
	char	*nl;
	int		tty_fd;

	setup_signal_handlers();
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
	{
		perror("open");
		return;
	}
	while (1)
	{
		nl = get_next_line(tty_fd);
		if (nl == NULL)
		{
			ft_putstr_fd(
				"Error : End Of File before finding here_doc LIMITER", 2);
			close(tty_fd);
			return ;
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter))
			&& nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			close(tty_fd);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(nl, fd);
		free(nl);
	}
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
	int		ret;

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
				exit(EXIT_FAILURE);
			}
			pid_t pid = fork();
			if (pid == 0)
				read_heredoc(((t_token_lst *)(current->content))->text, tmp_fd);
			else
			{
				ret = wait_status(0);
				close(tmp_fd);
				((t_token_lst *)(current->content))->type = redir_in;
				free(((t_token_lst *)(current->content))->text);
				((t_token_lst *)(current->content))->text = ft_strdup(tmp_name);
				if (ret)
					return (1);
				hd_no_2++;
			}
		}
		current = current->next;
	}
	return (0);
}