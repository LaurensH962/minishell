/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:21:40 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:21:40 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_up_shell(t_shell **shell, char **envp)
{
	*shell = ft_calloc(1, sizeof(t_shell));
	if (!*shell)
	{
		perror("minishell: malloc");
		exit(1);
	}
	if (copy_environ(envp, &(*shell)->env))
	{
		free(*shell);
		exit(1);
	}
	if (copy_environ(envp, &(*shell)->export))
	{
		free_array((*shell)->env, -1);
		free(*shell);
		exit(1);
	}
}

int	syntax_error_check(t_shell *shell, char *line)
{
	char	*syntax_error;

	syntax_error = syntax_checker(shell->tokens);
	if (syntax_error != NULL)
	{
		printf("%s\n", syntax_error);
		free_structs(shell);
		free(line);
		shell->status_last_command = 2;
		return (0);
	}
	return (1);
}

int	set_ast(t_shell *shell, char *line)
{
	t_ast	*ast;

	ast = parse(shell->tokens);
	if (!ast)
	{
		free_structs(shell);
		free(line);
		return (0);
	}
	free_tokens(shell);
	shell->node = ast;
	return (1);
}

void	set_values(int argc, char **argv)
{
	if (argc > 1)
	{
		printf("minishell does not take arguments.\n");
		exit(1);
	}
	(void)argv;
	rl_catch_signals = 0;
	g_rl_interrupted = 0;
}

int	new_readline(t_shell *shell, char **line)
{
	*line = readline("minishell: ");
	if (g_rl_interrupted == 2)
	{
		shell->status_last_command = 130;
		g_rl_interrupted = 0;
	}
	if (!(*line))
	{
		printf("exit\n");
		return (0);
	}
	return (1);
}
