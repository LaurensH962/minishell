/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:16:07 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:16:08 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(char *filename)
{
	struct stat	statbuf;

	if (stat(filename, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
		return (1);
	return (0);
}

int	check_file_access_read(char *filename, int i, t_shell *shell)
{
	if (access(filename, F_OK) == -1)
	{
		if (i == 3)
			report_error(filename, ": No such file or directory\n");
		if (i == 0)
			close_pipes_cleanup(shell);
		else
			return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		if (i == 3)
			report_error(filename, ": Permission denied\n");
		if (i == 0)
			close_pipes_cleanup(shell);
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
			if (i == 3)
				report_error(filename, ": Permission denied\n");
			if (i == 0)
				close_pipes_cleanup(shell);
			else
				return (1);
		}
	}
	return (0);
}

int	cd_access(char *path)
{
	if (access(path, F_OK) == -1)
	{
		cd_report_error("cd: ", path, ":  No such file or directory\n");
		return (1);
	}
	if (access(path, F_OK) == 0 && !is_directory(path))
	{
		cd_report_error("cd: ", path, ": Not a directory\n");
		return (1);
	}
	if (access(path, X_OK) == -1)
	{
		cd_report_error("cd: ", path, ": Permission denied\n");
		return (1);
	}
	return (0);
}

void	check_command_access(t_ast *node, t_shell *shell)
{
	if (node->cmd_path == NULL)
	{
		report_error(node->cmd, ": command not found\n");
		cleanup_all(shell);
		exit(127);
	}
	if (access(node->cmd_path, F_OK) == -1)
	{
		report_error(node->cmd, ": No such file or directory\n");
		cleanup_all(shell);
		exit(127);
	}
	if (access(node->cmd_path, X_OK) == -1)
	{
		report_error(node->cmd, ": Permission denied\n");
		cleanup_all(shell);
		exit(126);
	}
	if (access(node->cmd_path, X_OK) == 0 && is_directory(node->cmd))
	{
		report_error(node->cmd, ": Is a directory\n");
		cleanup_all(shell);
		exit(126);
	}
}

/*-fsanitize=address*/