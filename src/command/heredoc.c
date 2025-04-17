#include "minishell.h"

void	handle_heredoc(int *fd_read)
{
	if (*fd_read == -1)
	{
		perror("minishell: heredoc");
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

int		handle_heredoc_builtin(int *fd_read)
{
	if (*fd_read == -1)
	{
		perror("minishell: heredoc");
		return (1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
	return (0);
}

int	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	return(130);
}