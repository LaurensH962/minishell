#include "minishell.h"


void	handle_inputfile(int *fd_read, t_ast *node)
{
	check_file_access_read(node->redirections->file);
	*fd_read = open(node->redirections->file, O_RDONLY);
	if (*fd_read == -1)
	{
		perror("minishell: open");
		//cleanup_shell(shell);
		close(*fd_read);
		exit(1);
	}
	dup2(*fd_read, STDIN_FILENO);
	close(*fd_read);
}

void	handle_outputfile(int *fd_write, t_ast *node)
{
	check_file_access_write(node->redirections->file);
	if (node->redirections->type == NODE_APPEND)
		*fd_write = open(node->redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_write = open(node->redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_write == -1)
	{
		perror("minishell: open output file failed");
		//cleanup_shell(shell);
		close(*fd_write);
		exit(1);
	}
	dup2(*fd_write, STDOUT_FILENO);
	close(*fd_write);
}

/*char **copy_environ(char **env)
{
	int count;
	char **env_copy;
	int i ;

	count = 0;
	while(env[count] != NULL)
		count++;
	if (count == 0)
		return (NULL);
	env_copy = malloc(sizeof(char *) * (count + 1));
	if (env_copy == NULL)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	i = 0;
	while(i < count)
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
		{
			free_array(env_copy, i);
			perror("minishell: malloc");
			return (NULL);
		}
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}*/

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
	exit (exit_value);
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
