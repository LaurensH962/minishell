/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:15:22 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/22 14:31:55 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_arg_number(t_shell *shell, char *arg)
{
	long long	status;

	if (!ft_atoi_safe(arg, &status))
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_close_pipes(shell);
		cleanup_all(shell);
		exit(2);
	}
	if (status < 0 || status > 255)
		status = (status % 256 + 256) % 256;
	exit_close_pipes(shell);
	cleanup_all(shell);
	printf("exit\n");
	exit(status);
}

static void	exit_no_numeric_argument(t_shell *shell, char *arg)
{
	printf("exit\n");
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit_close_pipes(shell);
	cleanup_all(shell);
	exit(2);
}

static void	exit_no_args(t_shell *shell)
{
	long long	status;

	status = 0;
	exit_close_pipes(shell);
	cleanup_all(shell);
	printf("exit\n");
	exit(status);
}

void	ft_exit(char **args, t_shell *shell)
{
	if (args[1] == NULL)
		exit_no_args(shell);
	if (args[1])
	{
		if (args[2] != NULL && args[1])
		{
			printf("exit\n");
			if (is_number(args[1]))
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				return ;
			}
			else
			{
				exit_close_pipes(shell);
				cleanup_all(shell);
				ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
				exit(2);
			}
		}
		if (is_number(args[1]))
			exit_arg_number(shell, args[1]);
		else
			exit_no_numeric_argument(shell, args[1]);
	}
}
