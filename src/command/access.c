#include "minishell.h"

void	check_file_access_read(char *filename)
{
	if (access(filename, F_OK) == -1)
	{
		report_error(filename, "no such file or directory:");
		//cleanup_shell(shell);
		exit(1);
	}
	if (access(filename, R_OK) == -1)
	{
		report_error(filename, "permission denied:");
		//cleanup_shell(shell);
		exit(1);
	}
}

void	check_file_access_write(char *filename)
{
	if (access(filename, F_OK) != -1)
	{
		if (access(filename, W_OK) == -1)
		{
			report_error(filename, "permission denied:");
			//cleanup_shell(shell);
			exit(1);
		}
	}
}

void	check_command_access(t_ast *node)
{
	if (node->cmd_path == NULL)
	{
		report_error(node->cmd, "command not found");
		//cleanup_shell(shell);
		exit (127);
	}
	if (access(node->cmd_path, F_OK) == -1)
	{
		report_error(node->cmd, "no such file or directory:");
		//cleanup_shell(shell);
		exit (127);
	}
	if (access(node->cmd_path, X_OK) == -1)
	{
		report_error(node->cmd, "permission denied:");
		//cleanup_shell(shell);
		exit (126);
	}
}
