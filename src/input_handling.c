#include "minishell.h"


void	handle_sigint(int sig)
{
	(void)sig;
	g_rl_interrupted = 2;
	ft_putstr_fd("^C\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	//struct sigaction	sa_exit;

	//rl_done = 0;
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	free_tokens(t_shell *shell)
{
	t_token	*temp;

	while (shell->tokens)
	{
		temp = shell->tokens->next;
		free(shell->tokens->value);
		free(shell->tokens);
		shell->tokens = temp;
	}
}