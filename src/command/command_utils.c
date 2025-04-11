#include "minishell.h"

void	handle_inputfile(int *fd_read, t_ast *node)
{
	check_file_access_read(node->redirections->file);
	*fd_read = open(node->redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		//cleanup_shell(shell);
		close(*fd_read);
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

void	handle_outputfile(int *fd_write, t_ast *node)
{
	check_file_access_write(node->redirections->file);
	if (node->redirections->type == NODE_APPEND)
		*fd_write = open(node->redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_write = open(node->redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_write == -1)
	{
		perror("minishell: open output file failed");
		//cleanup_shell(shell);
		close(*fd_write);
		exit(1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
}

