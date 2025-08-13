/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:24:01 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/22 15:10:04 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_rl_interrupted = 0;

static void	set_up(char **argv, int argc, char **envp, t_shell **shell)
{
	set_values(argc, argv);
	setup_signal_handlers();
	set_up_shell(shell, envp);
}

static void	set_up_shell_values(t_shell **shell)
{
	(*shell)->pipe_count = 0;
	(*shell)->pipes = NULL;
	(*shell)->pid = NULL;
}

static int	return_exit_value(t_shell *shell)
{
	int		i;

	i = shell->status_last_command;
	cleanup_shell(shell);
	return (i);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*line;

	set_up(argv, argc, envp, &shell);
	while (1)
	{
		set_up_shell_values(&shell);
		if (!new_readline(shell, &line))
			break ;
		add_history(line);
		shell->tokens = lexer(line, shell);
		if (shell->tokens == NULL)
			break ;
		if (!syntax_error_check(shell, line))
			continue ;
		if (!set_ast(shell, line))
			break ;
		free(line);
		if (!command_path(shell->node, shell))
			execute_pipeline(shell);
		cleanup_pipes_pids(shell);
		cleanup_ast(&(shell->node));
	}
	return (return_exit_value(shell));
}
