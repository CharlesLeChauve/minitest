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
