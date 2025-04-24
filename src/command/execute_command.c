#include "minishell.h"

static int	handle_builtin_one_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	int backup_stdin;
	int backup_stdout;

	backup_stdin = dup(STDIN_FILENO);
	backup_stdout = dup(STDOUT_FILENO);
	if(check_if_builtin(node) && shell->pipe_count == 0)
    {
        if(handle_redirections_builtin(node, in_fd, out_fd))
        {
            shell->status_last_command = 1;
            return (1);
        }
        shell->status_last_command = execute_builtin(node, shell);
		dup2(backup_stdin, STDIN_FILENO);
		dup2(backup_stdout, STDOUT_FILENO);
		close(backup_stdin);
		close(backup_stdout);
		return (1);
    }
	close(backup_stdin);
	close(backup_stdout);
	return (0);
}

static void     handle_redirections(t_ast *node, int in_fd, int out_fd)
{
    int fd_read;
    int fd_write;
    t_redirect *redir;

    redir = node->redirections;
    redir_close(in_fd, out_fd);
    while(redir)
    {
        if (redir->type == NODE_REDIRECT_IN)
            handle_inputfile(&fd_read, redir);
        if (redir->type == NODE_REDIRECT_OUT)
            handle_outputfile(&fd_write, redir);
        if (redir->type == NODE_APPEND)
            handle_outputfile(&fd_write, redir);
        if (redir->type == NODE_HEREDOC)
            handle_heredoc(&redir->fd_heredoc);
        redir = redir->next;
    }
}

static void	create_child(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	pid_t pid;

	pid = fork();
	if(pid == -1)
	{
		perror("minishell: pipe");
		shell->status_last_command = 1;
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_redirections(node, in_fd, out_fd);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (node->cmd == NULL)
			//CLEANUP NEEDED
			exit(0);
		if (check_if_builtin(node))
			// CLEANUP NEEDED
			execute_builtin_exit(node, shell);
		check_command_access(node);
		execve(node->cmd_path, node->args, shell->export);
		perror("minishell: execve");
		// NEED CLEANUP HERE!!!!
		exit(1);
	}
	shell->pid[shell->pid_index++] = pid;
}

void   execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	if (handle_builtin_one_command(shell, node, in_fd, out_fd))
		return ;
	else
		create_child(shell, node, in_fd, out_fd);
}
