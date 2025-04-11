#include "minishell.h"

int	current_path(char *command)
{
	int	i;

	i = 0;
	if (command == NULL)
		return (0);
	if (command[i] == '.')
		return (1);
	else
		return (0);
}

int	command_is_path(char *argv)
{
	int	i;

	i = 0;
	if (argv == NULL)
		return (0);
	if (argv[i] == '/' && argv[i + 1] && argv[i + 1] != ' ' && argv[i
			+ 1] != '	')
		return (1);
	else
		return (0);
}
