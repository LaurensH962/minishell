#include "minishell.h"

int	check_file_access_read(char *filename, int i)
{
	if (access(filename, F_OK) == -1)
	{
		report_error(filename, "no such file or directory:");
		if (i == 0)
			exit(1);
		else
			return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		report_error(filename, "permission denied:");
		if (i == 0)
			exit(1);
		else
			return (1);
	}
	return (0);
}

int	check_file_access_write(char *filename, int i)
{
	if (access(filename, F_OK) != -1)
	{
		if (access(filename, W_OK) == -1)
		{
			report_error(filename, "permission denied:");
			if (i == 0)
				exit(1);
			else
				return (1);
		}
	}
	return (0);
}

void	check_command_access(t_ast *node)
{
	if (node->cmd_path == NULL)
	{
		report_error(node->cmd, "command not found");
		exit (127);
	}
	if (access(node->cmd_path, F_OK) == -1)
	{
		report_error(node->cmd, "no such file or directory:");
		exit (127);
	}
	if (access(node->cmd_path, X_OK) == -1)
	{
		report_error(node->cmd, "permission denied:");
		exit (126);
	}
}

/*-fsanitize=address*/