/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fill.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:40:04 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/20 10:55:20 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_command_node(t_ast **command_node, t_token *token, int *arg_count)
{
	(*command_node)->cmd = ft_strdup_protect(token->value);
	if (!(*command_node)->cmd)
	{
		free((*command_node)->args);
		free(*command_node);
		return (1);
	}
	(*command_node)->args[*arg_count] = ft_strdup_protect(token->value);
	if (!(*command_node)->args[*arg_count])
	{
		free((*command_node)->cmd);
		free((*command_node)->args);
		free(*command_node);
		return (1);
	}
	(*arg_count)++;
	return (0);
}

int	fill_args(t_ast **command_node, t_token *token, int *arg_count)
{
	(*command_node)->args[*arg_count] = ft_strdup_protect(token->value);
	if (!(*command_node)->args[*arg_count])
	{
		free((*command_node)->cmd);
		while (*arg_count > 0)
		{
			(*arg_count)--;
			free((*command_node)->args[*arg_count]);
		}
		free((*command_node)->args);
		free(*command_node);
		return (1);
	}
	(*arg_count)++;
	return (0);
}
