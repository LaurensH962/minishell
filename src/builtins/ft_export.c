/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:15:25 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:15:27 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_strings(char **arr)
{
	int		i;
	int		swapped;
	char	*temp;

	if (!arr)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (arr[i] && arr[i + 1])
		{
			if (ft_strcmp(arr[i], arr[i + 1]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				swapped = 1;
			}
			i++;
		}
	}
}

static int	print_export(t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	sort_strings(shell->export);
	while (shell->export[i] != NULL)
	{
		j = 0;
		printf("declare -x ");
		while (shell->export[i][j] && shell->export[i][j] != '=')
			printf("%c", shell->export[i][j++]);
		if (shell->export[i][j] == '=')
			print_export_if_equalsign(shell, &i, &j);
		else
			printf("\n");
		i++;
	}
	return (0);
}

static int	setenve_export(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = ft_substr(arg, 0, (equal) - arg);
		value = ft_strdup(equal + 1);
		if (!value || !key)
			return (perror_malloc_return());
		if (ft_setenv(key, value, &(shell->export), 1))
			return (perror_malloc_free_return(key, value));
		if (ft_setenv(key, value, &(shell->env), 1))
			return (perror_malloc_free_return(key, value));
		free(key);
		free(value);
	}
	else
	{
		if (ft_setenv(arg, "\0", &(shell->export), 0) != 0)
			return (1);
	}
	return (0);
}

static int	update_export(t_shell *shell, char **args)
{
	int	i;
	int	not_valid;
	int	error_malloc;

	i = 1;
	error_malloc = 0;
	not_valid = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			not_valid = 1;
		}
		else
		{
			if (setenve_export(shell, args[i]))
				error_malloc = 1;
		}
		i++;
	}
	return (not_valid || error_malloc);
}

int	ft_export(t_shell *shell, char **args)
{
	if (!args[1])
		return (print_export(shell));
	else
		return (update_export(shell, args));
}
