/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:15:30 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/22 11:18:28 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_for_key(const char *key, char ***envp, char *new_entry,
		int equal)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	if (*envp)
	{
		while ((*envp)[i])
		{
			if (ft_strncmp((*envp)[i], key, key_len) == 0
				&& ((*envp)[i][key_len] == '=' || (*envp)[i][key_len] == '\0'))
			{
				if (equal)
				{
					free((*envp)[i]);
					(*envp)[i] = new_entry;
				}
				else
					free(new_entry);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

static int	add_new_entry(char ***envp, char *new_entry)
{
	char	**new_env;
	int		i;

	i = 0;
	while (*envp && (*envp)[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return (perror_malloc_return());
	i = 0;
	while (*envp && (*envp)[i])
	{
		new_env[i] = ft_strdup((*envp)[i]);
		if (!new_env[i])
		{
			free_array(new_env, i);
			return (perror_malloc_return());
		}
		i++;
	}
	new_env[i++] = new_entry;
	new_env[i] = NULL;
	free_array((*envp), -1);
	*envp = new_env;
	return (0);
}

static char	*join_key_value_helper(const char *key, const char *value)
{
	char	*equal_string;
	char	*new_entry;

	equal_string = ft_strjoin(key, "=");
	if (!equal_string)
		return (NULL);
	new_entry = ft_strjoin(equal_string, value);
	if (!new_entry)
	{
		free(equal_string);
		return (NULL);
	}
	free(equal_string);
	return (new_entry);
}

static char	*join_key_value(const char *key, const char *value, int equal)
{
	char	*new_entry;

	if (*value != '\0')
	{
		new_entry = join_key_value_helper(key, value);
		if (!new_entry)
			return (NULL);
		return (new_entry);
	}
	else if (*value == '\0' && equal)
	{
		new_entry = ft_strjoin(key, "=");
		if (!new_entry)
			return (NULL);
		return (new_entry);
	}
	else
	{
		new_entry = ft_strdup(key);
		if (!new_entry)
			return (perror_return());
		return (new_entry);
	}
}

int	ft_setenv(const char *key, char *value, char ***envp, int equal)
{
	char	*new_entry;
	int		malloced;

	malloced = 0;
	if (!value)
	{
		value = pwd_not_set(value, &malloced);
		if (!value)
			return (perror_malloc_return());
	}
	new_entry = join_key_value(key, value, equal);
	if (malloced)
		free(value);
	if (!new_entry)
		return (1);
	if (search_for_key(key, envp, new_entry, equal))
		return (0);
	else
	{
		if (add_new_entry(envp, new_entry))
			return (free_return(new_entry));
		return (0);
	}
}
