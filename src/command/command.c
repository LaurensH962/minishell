
#include "minishell.h"

static void	prescan_redirections(t_ast *node, t_shell *shell)
{
	t_redirect *redir;
	int fd;
	int flags;

	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_REDIRECT_OUT || redir->type == NODE_APPEND)
		{
			if (redir->type == NODE_REDIRECT_OUT)
				flags = O_CREAT | O_WRONLY | O_TRUNC;
			else
				flags = O_CREAT | O_WRONLY | O_APPEND;
			if (check_file_access_write(redir->file, 3, shell))
				break ;
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
        break ;
      close(fd);
		}
		redir = redir->next;
	}
	if (node->left)
		prescan_redirections(node->left, shell);
	if (node->right)
		prescan_redirections(node->right, shell);
}

void	redir_close(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup2(out_fd, STDOUT_FILENO);
}


static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	if (node->type == NODE_PIPE)
	{
		// rec_index++;
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
	int i;
	int status;

	i = 0;
	while (i < shell->pid_index)
	{
		waitpid(shell->pid[i], &status, 0);
		if (WIFEXITED(status))
			shell->status_last_command = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) /*&& check_if_builtin(node) == 0*/)
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
	int in_fd;
	int out_fd;

	if (scan_heredocs(shell->node, shell) == -1)
	{
		g_rl_interrupted = 0;
		return ;
	}
	shell->pid = ft_calloc(1, sizeof(pid_t) * (shell->pipe_count + 1));
	if (!shell->pid)
	{
		perror("minishell: malloc");
		shell->status_last_command = 1;
		return ;
	}
	shell->pid_index = 0;
	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	//signal(SIGINT, SIG_IGN); // not sure if correctly placed
	
	if (shell->pipe_count > 0)
	{
		shell->pipes = malloc(sizeof(int *) * shell->pipe_count);
		if (!shell->pipes)
		{
			perror("minishell: malloc");
			shell->status_last_command = 1;
			return ;
		}
		shell->pipe_index = 0;
		shell->index = 0;
		int i = 0;
		while (i < shell->pipe_count)
		{
			shell->pipes[i] = malloc(sizeof(int) * 2);
			if (!shell->pipes[i])
			{
				perror("minishell: malloc");
				while (--i >= 0)
					free(shell->pipes[i]);
				free(shell->pipes);
				shell->pipes = NULL;
				shell->status_last_command = 1;
				return ;
			}
			i++;
		}
	}
	prescan_redirections(shell->node, shell);
	if (!shell->node->cmd && shell->node->type == NODE_COMMAND)
		return ;
	else
		execute_ast(shell, shell->node, in_fd, out_fd);
	int i = 0;
	while (i < (shell->pipe_count))
	{
		close(shell->pipes[i][0]);
		close(shell->pipes[i][1]);
		i++;
	}
	wait_for_children(shell);
	unlink_heredoc_fd(shell->node);
	setup_signal_handlers();
}
