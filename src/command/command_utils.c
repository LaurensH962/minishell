#include "minishell.h"

int	initialize_pid_array(t_shell *shell)
{
	shell->pid = ft_calloc(1, sizeof(pid_t) * (shell->pipe_count + 1));
	if (!shell->pid)
	{
		perror("minishell: malloc");
		shell->status_last_command = 1;
		return (1);
	}
	shell->pid_index = 0;
	return (0);
}

static int	malloc_pipe_fail(t_shell *shell, int i)
{
	perror("minishell: malloc");
	while (--i >= 0)
		free(shell->pipes[i]);
	free(shell->pipes);
	shell->pipes = NULL;
	shell->status_last_command = 1;
	return (1);
}

int	initialize_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->pipe_count > 0)
	{
		shell->pipes = malloc(sizeof(int *) * shell->pipe_count);
		if (!shell->pipes)
		{
			perror("minishell: malloc");
			shell->status_last_command = 1;
			return (1);
		}
		shell->pipe_index = 0;
		shell->index = 0;
		while (i < shell->pipe_count)
		{
			shell->pipes[i] = malloc(sizeof(int) * 2);
			if (!shell->pipes[i])
				return (malloc_pipe_fail(shell, i));
			i++;
		}
	}
	return (0);
}

void	redirections(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup2(out_fd, STDOUT_FILENO);
}

void	close_pipes_cleanup(t_shell *shell)
{
	close_pipes(shell);
	cleanup_all(shell);
	exit(1);
}
