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
    }
	return (0);
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
		if (node->cmd == NULL)
			exit(0);
		if (check_if_builtin(node))
			execute_builtin_exit(node, shell);
		check_command_access(node);
		execve(node->cmd_path, node->args, shell->export);
		perror("minishell: execve");
		exit(1);
	}
	shell->pid[shell->pid_index++] = pid;
}

static void   execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	pid_t pid;

	if (handle_builtin_one_command(shell, node, in_fd, out_fd))
		return ;
	else
		create_child(shell, node, in_fd, out_fd);
}
