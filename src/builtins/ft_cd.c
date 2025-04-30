#include "minishell.h"

static int cd_to_home(t_shell *shell)
{
	char	*home;
	int	i;

	i = 0;
	home = NULL;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			home = strdup(shell->env[i] + 5); // Skip "PWD="
			if (!home)
				return (perror_malloc_return());
			break;
		}
		i++;
	}
	if (!home)
		return (report_error("cd", ": HOME not set\n"), 1);
	if (cd_access(home))
		return (1);
	if (chdir(home) != 0)
		return (perror_free_return("chdir", home));
	free(home);
	return (0);
}

static char	*set_oldpwd(t_shell *shell)
{
	int i;
	char *oldpwd;

	i = 0;
	oldpwd = NULL;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "PWD=", 4) == 0)
		{
			oldpwd = strdup(shell->env[i] + 4); // Skip the "PWD=" part
			if (!oldpwd)
				perror("minishell: malloc");
			break ;
		}
		i++;
	}
	return(oldpwd);
}

char	*get_oldpwd(t_shell *shell)
{
	int i;
	char *oldpwd;
	int malloc;

	i = 0;
	malloc = 0;
	oldpwd = NULL;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "OLDPWD=", 7) == 0)
		{
			oldpwd = strdup(shell->env[i] + 7);
			if (!oldpwd)
			{
				perror("minishell: malloc");
				malloc = 1;
			}
			break ;
		}
		i++;
	}
	if (!oldpwd && malloc == 0)
		printf("minishell: cd: OLDPWD not set\n"); //report error ??
	return(oldpwd);
}

static int	cd_to_path(t_shell *shell, char *path)
{
	char	*oldpwd;
	char	*newpwd;

	if(strcmp(path, "-") == 0)
	{
		if (cd_minus(shell))
			return (1);
	}
	else
	{
		if (cd_access(path))
			return (1);
		if(chdir(path) != 0)
			return(perror_cd_return());
	}
	oldpwd = set_oldpwd(shell);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		perror("minishell: getcwd");
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, &shell->env, 1);
	if (newpwd)
		ft_setenv("PWD", newpwd, &shell->env, 1);
	free(oldpwd);
	free(newpwd);
	return (0);
}

int	ft_cd(t_shell *shell, t_ast *node)
{
	if (node->args[1] != NULL)
	{
		if(node->args[2] != NULL)
		{
			report_error(node->args[2], ": cd: too many arguments\n");
			return (1);
		}
	}
	if(node->args[1] == NULL || node->args[1][0] == '~')
		return (cd_to_home(shell));
	else
		return (cd_to_path(shell, node->args[1]));
}
