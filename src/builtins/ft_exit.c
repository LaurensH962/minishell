#include "minishell.h"

static void	exit_arg_number(t_shell *shell, char *arg)
{
	long long status;

	status = ft_atoi(arg);
	if (status > INT_MAX || status < INT_MIN)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_all(shell);
		exit(2);
	}
	if (status < 0 || status > 255)
		status = (status % 256 + 256) % 256;
	cleanup_all(shell);
	exit(status);
}

static void	exit_no_numeric_argument(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	cleanup_all(shell);
	exit(2);
}

static void exit_no_args(t_shell *shell)
{
	long long status;
	
	status = 0;
	cleanup_all(shell);
	exit(status);
}

void ft_exit(char **args, t_shell *shell)
{
	if (args[1] == NULL)
		exit_no_args(shell);
	if (args[1])
	{
		if (args[2] != NULL && args[1])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			if (is_number(args[1]))
				return ;
			else
			{
				cleanup_all(shell);
				exit (1);
			}
		}
		if(is_number(args[1]))
			exit_arg_number(shell, args[1]);
		else
			exit_no_numeric_argument(shell, args[1]);
	}
}
