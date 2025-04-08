#include "minishell.h"

int ft_echo(char **args, t_shell *shell)
{
	int i;
	int newline;

	newline = 1;
	i = 1;
	if (args[i] && !ft_strcmp(args[i], "-n")) // what if "-nnnnnnnnn"
	{
		newline = 0;
		while (args[i] && !ft_strcmp(args[i], "-n"))
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
	cleanup_shell(shell);
	return (0);
}


int	ft_cd(t_shell *shell, t_ast *node) //doesnt work
{
	char	*oldpwd;
	char	*newpwd;

	if(node->args[2] != NULL)
		report_error("cd", "to many arguments");
	if(node->args[1] == NULL)
		report_error("cd", "missing arguments");
	if (chdir(node->args[1]) != 0) 
	{
		perror("minishell: cd");
		return (1);
	}
	int i = 0;
	while (shell->env[i])
    {
        if (strncmp(shell->env[i], "PWD=", 4) == 0)
        {
            oldpwd = strdup(shell->env[i] + 4); // Skip the "PWD=" part
            break;
        }
        i++;
    }
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
    {
        perror("minishell: getcwd");
        return (1);
    }
	if (oldpwd)
        ft_setenv("OLDPWD", oldpwd, &shell->env);
    ft_setenv("PWD", newpwd, &shell->env);
    free(newpwd);
	cleanup_shell(shell);
    return (0);
}
/*int ft_cd(char *path)
{
	char *home;
	char *cwd;

	home = getenv("HOME");	//set home to enter home directory if cd is used without a path
	cwd = NULL;
	if(!path)
	{
		if(home && chdir(home) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	else
	{
		if (chdir(path) == -1)	//enter directory if it exists
		{
			perror("minishell: cd");
			return (1);
		}
	}
	cwd = getcwd(NULL, 0); //get new directory as a string e.g. "home/user/username"
	if (cwd != NULL)
	{
		printf("%s", cwd);
		free(cwd);
	}
	else
	{
		perror("minishell: getcwd");
		return (1);
	}
	return (0);
}*/

void ft_exit(char **args, t_shell *shell) //needs to exit minishell // doesnt work with more than 1 argument // needs to be execute in the parent process
{
	int status;

	status = 0;
	if (args == NULL || !args[1] || args[2])
	{
		cleanup_shell(shell);
		exit(status);
	}
	if(is_number(args[1]))
	{
		status = ft_atoi(args[1]);
		if (status < 0)
			status = 255;
		cleanup_shell(shell);
		exit(status);
	}
	else
	{
		cleanup_shell(shell);
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(255);
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
