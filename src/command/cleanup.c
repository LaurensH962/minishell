/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:16:25 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:16:27 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_redirect(t_redirect *redirect)
{
	t_redirect	*tmp;

	while (redirect)
	{
		tmp = redirect;
		if (tmp->type == NODE_HEREDOC)
		{
			if (tmp->fd_heredoc > 2)
				close(tmp->fd_heredoc);
		}
		redirect = redirect->next;
		free(tmp->file);
		free(tmp);
	}
}

void	cleanup_ast(t_ast **node)
{
	int	i;

	if ((*node) == NULL)
		return ;
	cleanup_ast(&(*node)->left);
	cleanup_ast(&(*node)->right);
	cleanup_redirect((*node)->redirections);
	free((*node)->cmd);
	free((*node)->cmd_path);
	if ((*node)->args)
	{
		i = 0;
		while ((*node)->args[i])
			free((*node)->args[i++]);
		free((*node)->args);
	}
	free((*node));
	(*node) = NULL;
}

void	cleanup_pipes_pids(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->pipe_count)
	{
		if (shell->pipes)
		{
			while (i < shell->pipe_count)
				free(shell->pipes[i++]);
			free(shell->pipes);
		}
	}
	if (shell->pid)
		free(shell->pid);
}

void	cleanup_shell(t_shell *shell)
{
	int	i;

	if (shell == NULL)
		return ;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	if (shell->export)
	{
		i = 0;
		while (shell->export[i])
			free(shell->export[i++]);
		free(shell->export);
	}
	free(shell);
}
