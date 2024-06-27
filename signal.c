#include "minishell.h"

void	handle_sigint(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	handle_sigpipe(int sig)
{
	(void) sig;
}
// void	handle_sigquit()
// {
// 	return ;
// }

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_pipe;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sa_pipe.sa_handler = handle_sigpipe;
	sigemptyset(&sa_pipe.sa_mask);
	sa_pipe.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa_pipe, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	// sa_quit.sa_handler = handle_sigquit;
    // sa_quit.sa_flags = 0;
    // sigemptyset(&sa_quit.sa_mask);
    // if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
    //     perror("sigaction");
    //     exit(EXIT_FAILURE);
    // }
}
