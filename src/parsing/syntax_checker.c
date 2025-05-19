/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:20:54 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:20:54 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*syntax_checker(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!prev || tokens->next->type == TOKEN_EOF
				|| tokens->next->type == TOKEN_PIPE)
				return ("syntax error");
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_APPEND
			|| tokens->type == TOKEN_HEREDOC)
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
				return ("syntax error");
		}
		else if (tokens->type == TOKEN_ERROR)
			return ("syntax error");
		prev = tokens;
		tokens = tokens->next;
	}
	return (NULL);
}
