#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

//struct termios	g_original_termios;

/* Save and restore terminal. in main we save the current terminal,
	since we are changing it to
not display control characters like ctrl-c. then we restore it with the TSCANOW (so like imediately)
to the original settings once we exit (with ctrl-d for example)*/
/* void	save_original_terminal(void)
{
	if (tcgetattr(STDIN_FILENO, &g_original_termios) == -1)
		perror("minishell: tcgetattr (saving original settings)");
}

void	restore_terminal(void)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_original_termios) == -1)
		perror("minishell: tcsetattr (restoring terminal)");
} */

/* suppress_output
	- Modifies terminal settings to hide control character echoes.
Disables the ECHOCTL flag, which prevents control characters
(like ^C for SIGINT) from being displayed in the terminal when typed. */
/* void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p))
		perror("minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p))
		perror("minishell: tcsetattr");
} */

/* we don't use sig for anything so just void it to mute warnings.
Inform readline we are on a new line.
Clear the current input line.
Redisplay the prompt so it looks always like we are in a proper shell lol.*/
void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void	handle_exit(int sig)
// {
// 	(void)sig;
// 	//restore_terminal();
// 	printf("\nexit\n");
// 	exit(0);
// }

/* 2 different signal handlers. 1 for Ctrl-C (SIGINT): sa_int and one to
ignore Ctrl-\ (SIG_QUIT). define the handler for when SIGINT is received
and sigemptyset the mask to prevent signalqueuing.
the flag will be set to SA_RESTART to ensure readline() will not fail but resumes.
then we do the same for the Ctrl-\. but instead of calling a function to handle the signal,we just
ignore it (SIG_IGN)
we also use an exit if the terminal is terminated in a different way to restore the terminal properly*/

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	//struct sigaction	sa_exit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	//sa_exit.sa_handler = handle_exit;
	//sigemptyset(&sa_exit.sa_mask);
	//sa_exit.sa_flags = 0;
	//sigaction(SIGHUP, &sa_exit, NULL);
	//sigaction(SIGTERM, &sa_exit, NULL);
}

/* first sigal handling is initiated. then while(1) to recreate shell. minishell> will be displayed
and stores result in line. then add line to history. if readline fails or ctrl-D(EOF),
	exit the shell*/
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_ast	*ast;
	t_shell *shell;
	char 	*syntax_error;


	argc = 0;
	argv = NULL;
	syntax_error = NULL;
	//save_original_terminal();
	setup_signal_handlers();
	//suppress_output();
	shell = ft_calloc(1, sizeof(t_shell));
	if (copy_environ(envp, &shell->env))
		exit (1);
	//export_default_variables(shell);
	//set_pwd(shell);
	//set_oldpwd(shell);
	//set_default_home(shell);
	while (1)
	{
		if (isatty(fileno(stdin)))
			line = readline("minishell: ");
		else
		{
			char *linetemp;
			linetemp = get_next_line(fileno(stdin));
			if (!linetemp)
				break;
			line = ft_strtrim(linetemp, "\n");
			free(linetemp);
		}
		/* char *str = ft_strdup(line);
		free(line);
		line = str; */
		//line = readline("minishell: ");
		if (!line)
		{
			//restore_terminal();
			//free_structs(shell);
			printf("exit\n");
			break ;
		}
		add_history(line);
		shell->tokens = lexer(line, shell);
		syntax_error = syntax_checker(shell->tokens);
		if (syntax_error != NULL)
		{
			printf("%s\n", syntax_error);
			free (syntax_error);
			free_structs(shell);
			//free(shell);
			free(line);
			continue ;
		}
		//print_tokens(shell->tokens);
		ast = parse(shell->tokens);
		if (!ast)
		{
			free_structs(shell);
			free(line);
			continue;
		}
		shell->node = ast;
		if (!set_command_path(shell->node, shell))
			execute_pipeline(shell);
		//printf ("cmd = %s\n", shell->node->cmd);
		//print_ast(ast, 1);
		//handle_path(shell, shell->node);
		//print_ast(ast, 1);
		//printf("\n");
		free_structs(shell);
		//free_array(shell->env);
		//printf("%i\n", shell->status_last_command);
		// printf("You entered: %s\n", line);
		free(line);
	}
	cleanup_shell(shell);
	//restore_terminal();
	return (0);
}
