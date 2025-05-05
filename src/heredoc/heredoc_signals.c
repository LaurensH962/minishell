#include "minishell.h"

void	init_heredoc_signals(struct sigaction *old_sa)
{
	struct sigaction new_sa;

	new_sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&new_sa.sa_mask);
	new_sa.sa_flags = 0;
	sigaction(SIGINT, &new_sa, old_sa);
	g_rl_interrupted = 0;
	rl_done = 0;
	rl_event_hook = heredoc_event_hook;
}

void	reset_heredoc_signals(struct sigaction *old_sa)
{
	rl_done = 0;
	rl_event_hook = NULL;
	sigaction(SIGINT, old_sa, NULL);
}