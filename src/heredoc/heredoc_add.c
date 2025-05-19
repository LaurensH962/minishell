/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_add.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:37 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:17:38 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_heredoc_lines(int fd, t_redirect *redir, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_rl_interrupted)
			break ;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		if (ft_strchr(line, '$'))
			line = check_expand_heredoc(line, shell);
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return (0);
}

int	fill_heredoc(t_redirect *redir, char *name, t_shell *shell)
{
	struct sigaction	old_sa;
	int					fd;

	init_heredoc_signals(&old_sa);
	fd = open(name, O_CREAT | O_RDWR | O_EXCL, 0644);
	if (fd == -1)
		return (fd);
	write_heredoc_lines(fd, redir, shell);
	reset_heredoc_signals(&old_sa);
	if (g_rl_interrupted == 2)
	{
		shell->status_last_command = 130;
		unlink(name);
		close(fd);
		return (-1);
	}
	close(fd);
	fd = open(name, O_RDWR, 0640);
	unlink(name);
	return (fd);
}

static int	create_heredoc(t_redirect *redir, t_shell *shell, unsigned long *i)
{
	char	temp_name[25];
	char	*num_str;

	(*i)++;
	ft_strlcpy(temp_name, "tempfile_", sizeof(temp_name));
	num_str = ft_itoa((int)(*i));
	if (num_str)
	{
		ft_strlcat(temp_name, num_str, sizeof(temp_name));
		free(num_str);
	}
	redir->fd_heredoc = fill_heredoc(redir, temp_name, shell);
	if (redir->fd_heredoc == -1)
		return (-1);
	return (0);
}

int	scan_heredocs(t_ast *node, t_shell *shell)
{
	t_redirect				*redir;
	static unsigned long	i;

	i = (unsigned long)&redir;
	if (!node)
		return (0);
	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_HEREDOC)
		{
			if (create_heredoc(redir, shell, &i) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	if (scan_heredocs(node->left, shell) == -1)
		return (-1);
	if (scan_heredocs(node->right, shell) == -1)
		return (-1);
	return (0);
}
