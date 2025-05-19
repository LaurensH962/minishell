/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:21:26 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:21:27 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_path(const char *dir, const char *cmd)
{
	char	*full_path;
	char	*tmp;

	if (!dir || !cmd)
		return (NULL);
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	if (!full_path)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (full_path);
}

static char	*find_envp_path(char **envp)
{
	char	*path_env;
	int		i;

	i = 0;
	path_env = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path_env);
}

static char	*find_command_path(char *cmd, char **paths, int *fail_flag)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths && paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (!full_path)
		{
			perror("minishell: malloc");
			if (*fail_flag)
				*fail_flag = 1;
			return (NULL);
		}
		if (access(full_path, F_OK) != -1)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, char **envp, int *fail_flag)
{
	char	*path_env;
	char	*full_path;
	char	**paths;

	path_env = find_envp_path(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		perror("minishell: malloc");
		if (fail_flag)
			*fail_flag = 1;
		return (NULL);
	}
	full_path = find_command_path(cmd, paths, fail_flag);
	if (!full_path)
	{
		free_split(paths);
		return (NULL);
	}
	free_split(paths);
	return (full_path);
}
