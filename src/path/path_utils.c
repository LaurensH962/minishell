/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:21:24 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:21:25 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	current_path(char *command)
{
	int	i;

	i = 0;
	if (command == NULL)
		return (0);
	if (command[i] == '.')
		return (1);
	else
		return (0);
}

static int	command_is_path(char *argv)
{
	int	i;

	i = 0;
	if (argv == NULL)
		return (0);
	if (argv[i] == '/' && argv[i + 1] && argv[i + 1] != ' ' && argv[i
			+ 1] != '	')
		return (1);
	else
		return (0);
}

static int	check_cmd(t_ast *node)
{
	if (node == NULL)
		return (0);
	if (!node->cmd && node->type != NODE_PIPE)
		return (0);
	if (node->cmd)
	{
		if (node->cmd[0] == '\0')
		{
			node->cmd_path = NULL;
			return (0);
		}
	}
	return (1);
}

static int	set_cmd_path(t_ast *node, int path, t_shell *shell, int fail_flag)
{
	if (node->type == NODE_COMMAND && node->cmd != NULL && path)
	{
		node->cmd_path = ft_strdup(node->cmd);
		if (!node->cmd_path)
			return (perror_malloc_return());
	}
	if (node->type == NODE_COMMAND && node->cmd != NULL && !path)
	{
		node->cmd_path = get_command_path(node->cmd, shell->env, &fail_flag);
		if (fail_flag == 1)
			return (1);
	}
	return (0);
}

int	command_path(t_ast *node, t_shell *shell)
{
	int	fail_flag;
	int	path;

	fail_flag = 0;
	if (!check_cmd(node))
		return (0);
	if (command_path(node->right, shell))
		return (1);
	if (command_path(node->left, shell))
		return (1);
	if ((current_path(node->cmd) || command_is_path(node->cmd)))
		path = 1;
	else
		path = 0;
	if (set_cmd_path(node, path, shell, fail_flag))
		return (1);
	return (0);
}
