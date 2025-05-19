#include "minishell.h"

void	handle_inputfile(int *fd_read, t_redirect *redirections, t_shell *shell)
{
	check_file_access_read(redirections->file, 0, shell);
	*fd_read = open(redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		close_pipes(shell);
		cleanup_all(shell);
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

void	handle_outputfile(int *fd_write, t_redirect *redirections,
		t_shell *shell)
{
	check_file_access_write(redirections->file, 0, shell);
	if (redirections->type == NODE_APPEND)
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (*fd_write == -1)
	{
		perror("minishell: open");
		close_pipes(shell);
		cleanup_all(shell);
		exit(1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
}

int	handle_inputfile_builtin(int *fd_read, t_redirect *redirections,
		t_shell *shell)
{
	if (check_file_access_read(redirections->file, 1, shell))
		return (1);
	*fd_read = open(redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		return (1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
	return (0);
}

int	handle_outputfile_builtin(int *fd_write, t_redirect *redirections,
		t_shell *shell)
{
	if (check_file_access_write(redirections->file, 1, shell))
		return (1);
	if (redirections->type == NODE_APPEND)
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (*fd_write == -1)
	{
		perror("minishell: open");
		close(*fd_write);
		return (1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
	return (0);
}

int	handle_redirections_builtin(t_ast *node, int in_fd, int out_fd,
		t_shell *shell)
{
	int			fd_read;
	int			fd_write;
	t_redirect	*redir;
	int			result;

	result = 0;
	redir = node->redirections;
	redirections(in_fd, out_fd);
	while (redir)
	{
		if (redir->type == NODE_REDIRECT_IN)
			result = handle_inputfile_builtin(&fd_read, redir, shell);
		if (redir->type == NODE_REDIRECT_OUT)
			result = handle_outputfile_builtin(&fd_write, redir, shell);
		if (redir->type == NODE_APPEND)
			result = handle_outputfile_builtin(&fd_write, redir, shell);
		if (redir->type == NODE_HEREDOC)
			result = handle_heredoc_builtin(&redir->fd_heredoc);
		if (result != 0)
			return (1);
		redir = redir->next;
	}
	return (0);
}
