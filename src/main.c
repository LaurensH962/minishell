#include "minishell.h"

volatile sig_atomic_t g_rl_interrupted = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_ast	*ast;
	t_shell *shell;
	char 	*syntax_error;


	(void)argc;
	(void)argv;
	syntax_error = NULL;
	rl_catch_signals = 0;
	g_rl_interrupted = 0;
	setup_signal_handlers();
	shell = ft_calloc(1, sizeof(t_shell));
	if (copy_environ(envp, &shell->env))
		exit (1);
	if (copy_environ(envp, &shell->export))
		exit (1);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("stdin is ok\n");
		else
			printf("stdin is closed or redirected!\n");
		if (isatty(fileno(stdin)))
		{
			printf("stdin is ok");
			line = readline("minishell: ");
		}
		else
		{
			char *linetemp;
			linetemp = get_next_line(fileno(stdin));
			if (!linetemp)
				break;
			line = ft_strtrim(linetemp, "\n");
			free(linetemp);
		}
		shell->pipe_count = 0;
		//line = readline("minishell: ");
		if (g_rl_interrupted == 2)
		{
			shell->status_last_command = 130;
			g_rl_interrupted = 0;
		}
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		add_history(line);
		shell->tokens = lexer(line, shell);
		//print_tokens(shell->tokens);
		syntax_error = syntax_checker(shell->tokens);
		if (syntax_error != NULL)
		{
			printf("%s\n", syntax_error);
			free (syntax_error);
			free_structs(shell);
			free(line);
			shell->status_last_command = 2;
			continue ;
		}
		ast = parse(shell->tokens);
		//print_ast(ast, 1);
		if (!ast)
		{
			free_structs(shell);
			free(line);
			continue;
		}
		free_tokens(shell);
		shell->node = ast;
		if (!set_command_path(shell->node, shell))
			execute_pipeline(shell);
		cleanup_ast(&(shell->node));		
		free(line);
	}
	cleanup_shell(shell);
	return (0);
}
