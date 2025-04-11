#include "minishell.h"

void	execute_builtin(t_ast *node, t_shell *shell)
{
	int exit_value;

	exit_value = 1;
	if (ft_strcmp(node->cmd ,"echo") == 0)
		exit_value = ft_echo(node->args);
	else if (ft_strcmp(node->cmd , "cd") == 0)
		exit_value = ft_cd(shell, node);
	else if (ft_strcmp(node->cmd ,"exit") == 0)
		ft_exit(node->args);
	else if (ft_strcmp(node->cmd  ,"pwd") == 0)
		exit_value = ft_pwd();
	else if (ft_strcmp(node->cmd , "export") == 0)
		exit_value = ft_export(shell, node->args);
	else if (ft_strcmp(node->cmd , "unset") == 0)
		exit_value = ft_unset(shell, node->args);
	else if (ft_strcmp(node->cmd ,"env") == 0)
		exit_value = ft_env(shell);
	exit(exit_value);
}

int	execute_builtin_env(t_ast *node, t_shell *shell)
{
	int exit_value;

	exit_value = 1;
	if (ft_strcmp(node->cmd , "cd") == 0)
		exit_value = ft_cd(shell, node);
	else if (ft_strcmp(node->cmd , "export") == 0)
		exit_value = ft_export(shell, node->args);
	else if (ft_strcmp(node->cmd , "unset") == 0)
		exit_value = ft_unset(shell, node->args);
	return (exit_value);
}


int	check_if_builtin(t_ast *node)
{
	if (ft_strcmp(node->cmd ,"echo") == 0)
		return (1);
	else if (ft_strcmp(node->cmd , "cd") == 0)
		return (1);
	else if (ft_strcmp(node->cmd ,"exit") == 0)
		return (1);
	else if (ft_strcmp(node->cmd  ,"pwd") == 0)
		return (1);
	else if (ft_strcmp(node->cmd , "export") == 0)
		return (1);
	else if (ft_strcmp(node->cmd , "unset") == 0)
		return (1);
	else if (ft_strcmp(node->cmd ,"env") == 0)
		return (1);
	else
		return (0);
}

int check_if_env_builtin(t_ast *node)
{
    if(node->args == NULL)
        return (0);
    if(node->args[0] == NULL)
        return (0);
    if(ft_strcmp(node->args[0], "cd") == 0)
        return (1);
    if(ft_strcmp(node->args[0], "unset") == 0)
        return (1);
    if(ft_strcmp(node->args[0], "export") == 0)
        return (1);
    else
        return (0);
}