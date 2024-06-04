#include "minishell.h"

void	read_heredoc(char *limiter)
{
	char	*nl;

	while (1)
	{
		nl = get_next_line(0);
		if (nl == NULL)
		{
			ft_putstr_fd(
				"Error : End Of File before finding here_doc LIMITER", 2);
			exit(EXIT_FAILURE);
		}
		if (!ft_strncmp(nl, limiter, ft_strlen(limiter))
			&& nl[ft_strlen(limiter)] == '\n')
		{
			free(nl);
			exit(0);
		}
		ft_putstr_fd(nl, 1);
		free(nl);
	}
}

void	heredoc_handle(char *limiter)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		exit(-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		read_heredoc(limiter);
	}
	else if (pid > 0)
	{
		waitpid(pid, &(int){0}, 0);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
	}
}
