/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:07 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/22 14:33:48 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i] != NULL)
		free(split[i++]);
	free(split);
}

void	free_array(char **array, int len)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] && (len == -1 || i < len))
		free(array[i++]);
	free(array);
}

void	cleanup_all(t_shell *shell)
{
	unlink_heredoc_fd(shell->node);
	cleanup_pipes_pids(shell);
	cleanup_ast(&(shell->node));
	cleanup_shell(shell);
}

void	close_and_clean(t_shell *shell)
{
	close_pipes(shell);
	cleanup_all(shell);
}
