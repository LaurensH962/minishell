/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:17:39 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:17:40 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char(char *s, char c, int *i)
{
	char	str[2];
	char	*result;

	str[0] = c;
	str[1] = '\0';
	result = ft_strjoin(s, str);
	free(s);
	(*i)++;
	return (result);
}

char	*ft_strjoin_free(char *a, char *b)
{
	char	*tmp;

	tmp = ft_strjoin(a, b);
	free(a);
	return (tmp);
}

static char	*expand_special_heredoc(char *result, int *i, t_shell *shell)
{
	char	*status_str;

	status_str = ft_itoa(shell->status_last_command);
	result = ft_strjoin_free(result, status_str);
	free(status_str);
	*i += 2;
	return (result);
}

char	*expand_dollar(char *line, int *i, t_shell *shell)
{
	char	*result;
	char	*var_name;
	char	*env_value;
	int		start;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	if (line[*i + 1] == '?')
		result = expand_special_heredoc(result, i, shell);
	else
	{
		(*i)++;
		start = *i;
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		var_name = ft_substr(line, start, *i - start);
		env_value = get_env_value(shell->env, var_name);
		if (!env_value)
			env_value = "";
		result = ft_strjoin_free(result, env_value);
		free(var_name);
	}
	return (result);
}

char	*check_expand_heredoc(char *line, t_shell *shell)
{
	int		i;
	char	*expanded;
	char	*temp;
	char	*expanded_part;

	i = 0;
	expanded = ft_strdup("");
	if (!expanded)
		return (free(line), NULL);
	while (line[i])
	{
		if (line[i] == '$' && (line[i + 1] == '?' || ft_isalpha(line[i + 1])
				|| line[i + 1] == '_'))
		{
			expanded_part = expand_dollar(line, &i, shell);
			temp = expanded;
			expanded = ft_strjoin(expanded, expanded_part);
			free(temp);
			free(expanded_part);
		}
		else
			expanded = append_char(expanded, line[i], &i);
	}
	free(line);
	return (expanded);
}
