#include "minishell.h"

int	check_redirections(t_shell *shell, t_redirect *redir)
{
	int	fd;
	int	flags;

	if (redir->type == NODE_REDIRECT_IN)
	{
		if (check_file_access_read(redir->file, 3, shell))
			return (set_status_last_command_return(shell, 2));
	}
	if (redir->type == NODE_REDIRECT_OUT || redir->type == NODE_APPEND)
	{
		if (redir->type == NODE_REDIRECT_OUT)
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		else
			flags = O_CREAT | O_WRONLY | O_APPEND;
		if (check_file_access_write(redir->file, 3, shell))
			return (set_status_last_command_return(shell, 2));
		fd = open(redir->file, flags, 0644);
		if (fd == -1)
		{
			perror("minishell: open");
			return (1);
		}
		close(fd);
	}
	return (0);
}

static int	prescan_redirections(t_ast *node, t_shell *shell)
{
	t_redirect	*redir;
	int			result;

	redir = node->redirections;
	while (redir)
	{
		result = check_redirections(shell, redir);
		if (result == 1)
			set_status_last_command_return(shell, 1);
		if (result == 2)
			break ;
		redir = redir->next;
	}
	if (node->left)
	{
		if (prescan_redirections(node->left, shell))
			return (1);
	}
	if (node->right)
	{
		if (prescan_redirections(node->right, shell))
			return (1);
	}
	return (0);
}

static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	if (node->type == NODE_PIPE)
	{
		if (pipe(shell->pipes[shell->pipe_index]) == -1)
		{
			perror("minishell: pipe");
			return ;
		}
		shell->pipe_index++;
		execute_ast(shell, node->left, in_fd, shell->pipes[shell->pipe_index
			- 1][1]);
		shell->index++;
		execute_ast(shell, node->right, shell->pipes[shell->pipe_index
			- shell->index][0], out_fd);
	}
	else if (node->type == NODE_COMMAND)
		execute_command(shell, node, in_fd, out_fd);
}

static void	wait_for_children(t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < shell->pid_index)
	{
		waitpid(shell->pid[i], &status, 0);
		if (WIFEXITED(status))
			shell->status_last_command = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->status_last_command = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				printf("\n");
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
		}
		i++;
	}
}

void	execute_pipeline(t_shell *shell)
{
	int	in_fd;
	int	out_fd;

	if (heredoc_interrupt(shell))
		return ;
	if (initialize_pid_array(shell))
		return ;
	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	if (initialize_pipes(shell))
		return ;
	if (prescan_redirections(shell->node, shell) == 1)
		return ;
	if (!shell->node->cmd && shell->node->type == NODE_COMMAND)
		return ;
	else
	{
		signal(SIGINT, SIG_IGN);
		execute_ast(shell, shell->node, in_fd, out_fd);
	}
	close_pipes(shell);
	wait_for_children(shell);
	unlink_heredoc_fd(shell->node);
	setup_signal_handlers();
}
