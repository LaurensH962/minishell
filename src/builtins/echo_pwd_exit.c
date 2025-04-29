#include "minishell.h"

static int	echo_n_check(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-' && str[i + 1] && str[i + 1] == 'n')
	{
		i++;
		while(str[i] && str[i] == 'n')
			i++;
		if (str[i] && str[i] != 'n')
			return (0);
		else
			return (1);
	}
	return (0);
}

int ft_echo(char **args)
{
	int i;
	int newline;

	newline = 1;
	i = 1;
	if (args[i] && echo_n_check(args[i]))
	{
		newline = 0;
		while (args[i] && echo_n_check(args[i]))
			i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

void ft_exit(char **args, t_shell *shell)
{
	int status;

	status = 0;
	if (args[1] == NULL)
	{
		cleanup_all(shell);
		exit(status);
	}
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
		{
			status = ft_atoi(args[1]);
			if (status < 0 || status > 255)
				status = (status % 256 + 256) % 256;
			cleanup_all(shell);
			exit(status);
		}
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			cleanup_all(shell);
			exit(2);
		}
	}
}

int ft_pwd(void)
{
	char *cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
	return (0);
}
