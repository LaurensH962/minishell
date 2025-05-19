/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:18:06 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:18:06 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_special_cases(const char *input, size_t *pos, t_lexer *lexer)
{
	size_t	start;
	char	*result;

	start = *pos + 1;
	if (input[start] == '?')
	{
		*pos = start + 1;
		result = ft_itoa(lexer->shell->status_last_command);
		if (!result)
			return (perror_return());
		return (result);
	}
	if (input[start] == '\0' || ft_isspace(input[start]) || input[start] == '"'
		|| input[start] == '\'')
	{
		(*pos)++;
		result = ft_strdup("$");
		if (!result)
			return (perror_return());
		return (result);
	}
	return (NULL);
}

bool	extract_var_name(const char *input, size_t *start, char *var_name)
{
	size_t	i;

	i = 0;
	while (input[*start] && (ft_isalnum(input[*start]) || input[*start] == '_')
		&& i < 255)
		var_name[i++] = input[(*start)++];
	if (i == 0 && input[*start] != '\'' && input[*start] != '"')
		return (false);
	var_name[i] = '\0';
	return (true);
}

char	*lookup_and_expand(char *var_name, t_lexer *lexer)
{
	char	*env_value;
	char	*result;

	env_value = get_env_value(lexer->shell->env, var_name);
	if (env_value)
	{
		result = ft_strdup(env_value);
		if (!result)
			return (perror_return());
		return (result);
	}
	return (NULL);
}
