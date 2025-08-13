/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:21:02 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:21:02 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirect(t_token_type type)
{
	return (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	is_delimiter(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

char	*ft_strjoin_minishell(const char *s1, char *s2, char *input)
{
	char	*str;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2 && s1 && s1[0] == '\0')
		return (NULL);
	else if (!s2)
		return (ft_strdup_protect(s1));
	len = ft_strlen(input) + ft_strlen(s1) + ft_strlen(s2) + 1;
	str = ft_calloc(1, sizeof(char) * len);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len);
	ft_strlcat(str, s2, len);
	return (str);
}

void	free_structs(t_shell *shell)
{
	t_token	*temp;

	while (shell->tokens)
	{
		temp = shell->tokens->next;
		free(shell->tokens->value);
		free(shell->tokens);
		shell->tokens = temp;
	}
	if (shell->node)
		cleanup_ast(&(shell->node));
}

char	*ft_strdup_protect(const char *s)
{
	char	*p;
	size_t	len;

	len = ft_strlen(s) + 1;
	p = (char *)malloc(len);
	if (p == NULL)
		return (perror_return());
	ft_memcpy(p, s, len);
	return (p);
}
