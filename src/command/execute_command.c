#include "minishell.h"

static void	redirect_in_out(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}


static int	handle_builtin_one_command(t_shell *shell, t_ast *node, int in_fd,
		int out_fd)
{
	int	backup_stdin;
	int	backup_stdout;

	backup_stdin = dup(STDIN_FILENO);
	backup_stdout = dup(STDOUT_FILENO);
	if (check_if_builtin(node) && shell->pipe_count == 0)
	{
		if (handle_redirections_builtin(node, in_fd, out_fd, shell))
		{
			shell->status_last_command = 1;
			redirect_in_out(backup_stdin, backup_stdout);
			return (1);
		}
		shell->status_last_command = execute_builtin(node, shell, backup_stdin,
				backup_stdout);
		redirect_in_out(backup_stdin, backup_stdout);
		return (1);
	}
	close(backup_stdin); //not sure if needed
	close(backup_stdout); //not sure if needed
	return (0);
}

static void	create_child(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: pipe");
		shell->status_last_command = 1;
		return ;
	}
	if (pid == 0)
		child_process(shell, node, in_fd, out_fd);
	shell->pid[shell->pid_index++] = pid;
}

void	execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	if (!handle_builtin_one_command(shell, node, in_fd, out_fd))
		create_child(shell, node, in_fd, out_fd);
}