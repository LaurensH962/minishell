/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:16:16 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/20 12:59:05 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < (shell->pipe_count))
	{
		close(shell->pipes[i][0]);
		close(shell->pipes[i][1]);
		i++;
	}
}

static void	handle_redirections(t_ast *node, int in_fd, int out_fd,
		t_shell *shell)
{
	int			fd_read;
	int			fd_write;
	t_redirect	*redir;

	redir = node->redirections;
	redirections(in_fd, out_fd);
	while (redir)
	{
		if (redir->type == NODE_REDIRECT_IN)
			handle_inputfile(&fd_read, redir, shell);
		if (redir->type == NODE_REDIRECT_OUT)
			handle_outputfile(&fd_write, redir, shell);
		if (redir->type == NODE_APPEND)
			handle_outputfile(&fd_write, redir, shell);
		if (redir->type == NODE_HEREDOC)
			handle_heredoc(&redir->fd_heredoc);
		redir = redir->next;
	}
}

void	child_process(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_redirections(node, in_fd, out_fd, shell);
	if (node->cmd == NULL)
	{
		close_pipes(shell);
		cleanup_all(shell);
		exit(0);
	}
	if (check_if_builtin(node))
		execute_builtin_exit(node, shell);
	check_command_access(node, shell);
	unlink_heredoc_fd(shell->node);
	close_pipes(shell);
	execve(node->cmd_path, node->args, shell->export);
	perror("minishell: execve");
	cleanup_all(shell);
	exit(1);
}
