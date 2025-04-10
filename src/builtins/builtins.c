#include "minishell.h"

int ft_echo(char **args)
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
	return (0);
}


int	ft_cd(t_shell *shell, t_ast *node)
{
	char	*oldpwd;
	char	*newpwd;
	char	*home;
	int i = 0;

	oldpwd = NULL;
	home = NULL;
	if(node->args[2] != NULL)
	{
		report_error("cd", "to many arguments");
		return (1);
	}
	if(node->args[1] == NULL)
	{
    	while (shell->env[i])
		{
			if (strncmp(shell->env[i], "HOME=", 5) == 0)
			{
				home = strdup(shell->env[i] + 5); // Skip "PWD="
				if (!home)
				{
					perror("minishell: malloc");
					return (1);
				}
				break;
			}
			i++;
		}
		chdir(home);
		free(home);
		return (0);
	}
	if (chdir(node->args[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	i = 0;
	while (shell->env[i])
    {
        if (strncmp(shell->env[i], "PWD=", 4) == 0)
        {
            oldpwd = strdup(shell->env[i] + 4); // Skip the "PWD=" part
            break ;
        }
        i++;
    }
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
    {
        perror("minishell: getcwd");
		if (oldpwd)
			free(oldpwd);
        return (1);
    }
	if (oldpwd)
	{
        ft_setenv("OLDPWD", oldpwd, &shell->env);
		free(oldpwd);
	}
    ft_setenv("PWD", newpwd, &shell->env);
    free(newpwd);
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

void ft_exit(char **args)
{
	int status;

	status = 0;
	if (args[1] == NULL)
	{
		//cleanup_shell(shell);
		exit(status);
	}
	if (args[1])
	{
		if (args[2] != NULL)
		{
			printf("minishell: exit: too many arguments\n");
			exit (1);
		}
		if(is_number(args[1]))
		{
			status = ft_atoi(args[1]);
			if (status < 0 || status > 255)
				status = (status % 256 + 256) % 256;
			exit(status);
		}
		else
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
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

void	set_pwd(t_shell *shell)
{
	int		i;
	int		j;
	char	*home;

	j = 0;
	i = 0;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			home = strdup(shell->env[i] + 5); // Skip "HOME="
			if (!home)
			{
				perror("minishell: malloc");
				return ;
			}
			while(shell->env[j])
			{
				if(strncmp(shell->env[j], "PWD=", 4) == 0)
				{
					free(shell->env[j]);
					shell->env[j] = ft_strjoin("PWD=", home);
					if (!shell->env[j])
					{
						perror("minishell: malloc");
						free(home);
						return ;
					}
					break ;
				}
				j++;
			}
			free(home);
			break ;
		}
		i++;
	}
}

void	set_oldpwd(t_shell *shell)
{
	int		i;
	int		j;
	char	*home;

	j = 0;
	i = 0;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			home = strdup(shell->env[i] + 5); // Skip "HOME="
			if (home)
			{
				perror("minishell: malloc");
				return ;
			}
			while(shell->env[j])
			{
				if(strncmp(shell->env[j], "OLDPWD=", 4) == 0)
				{
					free(shell->env[j]);
					shell->env[j] = ft_strjoin("OLDPWD=", home);
					if (!shell->env[j])
					{
						perror("minishell: malloc");
						free(home);
						return ;
					}
					break ;
				}
				j++;
			}
			//if not found because unset, would I need to append it ?
			free(home);
			break ;
		}
		i++;
	}
}