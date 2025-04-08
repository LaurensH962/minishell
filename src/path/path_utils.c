#include "minishell.h"

static int	current_path(char *command)
{
	int	i;

	i = 0;
	while (command[i] && command[i] == ' ' && command[i] == '	')
		i++;
	if (command[i] == '.')
		return (1);
	else
		return (0);
}

static int	command_is_path(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] == ' ' || argv[i] == '	')
		i++;
	if (argv[i] == '/' && argv[i + 1] && argv[i + 1] != ' ' && argv[i
			+ 1] != '	')
		return (1);
	else
		return (0);
}

void	handle_path(t_shell *shell, t_ast *node)
{
	if (current_path(node->cmd) || command_is_path(node->cmd))
		return ;
	else
		set_command_path(node, shell);
}
