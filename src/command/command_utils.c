#include "minishell.h"

void	handle_inputfile(int *fd_read, t_redirect *redirections)
{
	check_file_access_read(redirections->file, 0);
	*fd_read = open(redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		close(*fd_read);
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

void	handle_outputfile(int *fd_write, t_redirect *redirections)
{
	check_file_access_write(redirections->file, 0);
	if (redirections->type == NODE_APPEND)
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_write == -1)
	{
		perror("minishell: open output file failed");
		close(*fd_write);
		exit(1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
}

int	handle_inputfile_builtin(int *fd_read, t_redirect *redirections)
{
	if (check_file_access_read(redirections->file, 1))
		return (1);
	*fd_read = open(redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		close(*fd_read);
		return(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
	return (0);
}

int	handle_outputfile_builtin(int *fd_write, t_redirect *redirections)
{
	if(check_file_access_write(redirections->file, 1))
		return (1);
	if (redirections->type == NODE_APPEND)
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_write = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_write == -1)
	{
		perror("minishell: open output file failed");
		close(*fd_write);
		return (1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
	return (0);
}

int     handle_redirections_builtin(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
    int fd_read;
    int fd_write;
    int heredoc_pipe[2];
    t_redirect *redir;

    redir = node->redirections;
    redir_close(in_fd, out_fd);
    while(redir)
    {
        if (redir->type == NODE_REDIRECT_IN)
            return (handle_inputfile_builtin(&fd_read, redir));
        if (redir->type == NODE_REDIRECT_OUT)
            return (handle_outputfile_builtin(&fd_write, redir));
        if (redir->type == NODE_APPEND)
    		return (handle_outputfile_builtin(&fd_write, redir));
        if (redir->type == NODE_HEREDOC)
            handle_heredoc(heredoc_pipe, node, shell);
        redir = redir->next;
    }
	return (0);
}

