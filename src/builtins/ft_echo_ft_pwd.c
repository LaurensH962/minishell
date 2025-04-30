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
