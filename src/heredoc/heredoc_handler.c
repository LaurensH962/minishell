/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:42 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:17:42 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(int *fd_read)
{
	if (*fd_read == -1)
	{
		perror("minishell: heredoc");
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

int	handle_heredoc_builtin(int *fd_read)
{
	if (*fd_read == -1)
	{
		perror("minishell: heredoc");
		return (1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
	return (0);
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "^C", 2);
	g_rl_interrupted = 2;
	rl_done = 1;
}

int	heredoc_event_hook(void)
{
	return (0);
}

void	unlink_heredoc_fd(t_ast *node)
{
	t_redirect	*redir;

	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_HEREDOC)
			close(node->redirections->fd_heredoc);
		redir = redir->next;
	}
	if (node->left)
		unlink_heredoc_fd(node->left);
	if (node->right)
		unlink_heredoc_fd(node->right);
}
