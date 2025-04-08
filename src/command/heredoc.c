#include "minishell.h"

void	handle_heredoc(int *heredoc_pipe, t_ast *node, t_shell *shell)
{
	if (pipe(heredoc_pipe) == -1)
	{
		perror("minishell: pipe");
		cleanup_shell(shell);
		exit(1);
	}
	write(heredoc_pipe[1], node->redirections->file, strlen(node->redirections->file));
    close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
    close(heredoc_pipe[0]);
}
