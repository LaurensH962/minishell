#include "minishell.h"

int is_directory(char *filename) 
{
    struct stat statbuf;

    if (stat(filename, &statbuf) != 0) 
        return (0);
    if (S_ISDIR(statbuf.st_mode)) 
        return (1);
    return 0;
}

int	check_file_access_read(char *filename, int i, t_shell *shell)
{
	if (access(filename, F_OK) == -1)
	{
		report_error(filename, "No such file or directory");
		if (i == 0)
		{
			cleanup_all(shell);
			exit(1);
		}
		else
			return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		report_error(filename, "Permission denied");
		if (i == 0)
		{
			cleanup_all(shell);
			exit(1);
		}
		else
			return (1);
	}
	return (0);
}

int	check_file_access_write(char *filename, int i, t_shell *shell)
{
	if (access(filename, F_OK) != -1)
	{
		if (access(filename, W_OK) == -1)
		{
			if (i == 0 || i == 1)
				report_error(filename, "Permission denied");
			if (i == 0)
			{
				cleanup_all(shell);
				exit(1);
			}
			else
				return (1);
		}
	}
	return (0);
}

void	check_command_access(t_ast *node, t_shell *shell)
{
	if (node->cmd_path == NULL)
	{
		report_error(node->cmd, "command not found");
		cleanup_all(shell);
		exit (127);
	}
	if (access(node->cmd_path, F_OK) == -1)
	{
		report_error(node->cmd, "No such file or directory");
		cleanup_all(shell);
		exit (127);
	}
	if (access(node->cmd_path, X_OK) == -1)
	{
		report_error(node->cmd, "Permission denied");
		cleanup_all(shell);
		exit (126);
	}
	if (access(node->cmd_path, X_OK) == 0 && is_directory(node->cmd))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->cmd, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		cleanup_all(shell);
		exit(126);
	}
}

/*-fsanitize=address*/