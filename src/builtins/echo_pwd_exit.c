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

void ft_exit(char **args)
{
	int status;

	status = 0;
	if (args[1] == NULL)
		exit(status);
	if (args[1])
	{
		if (args[2] != NULL && args[1])
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

/*void	set_pwd(t_shell *shell)
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
				perror("minishell1: malloc");
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
						perror("minishell2: malloc");
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
			if (!home)
			{
				perror("minishell3: malloc");
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
						perror("minishell4: malloc");
						free(home);
						return ;
					}
					break ;
				}
				j++;
			}
			//if not found because unset, would I need to append it?
			free(home);
			break ;
		}
		i++;
	}
}

void	set_default_home(t_shell *shell)
{
	char *home;
	int i;

	i = 0;
	home = NULL;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			home = strdup(shell->env[i] + 5); // Skip "HOME="
			if (!home)
				perror("minishell: malloc");
			break;
		}
		i++;
	}
	if (home)
		chdir(home);
	free(home);
}*/
