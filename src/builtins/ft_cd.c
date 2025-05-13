#include "minishell.h"

static int	cd_to_home(t_shell *shell)
{
	char	*home;
	char	*old_pwd;

	home = NULL;
	old_pwd = NULL;
	if (set_home_oldpwd(shell, &home, &old_pwd))
		return (1);
	if (!home)
		return (cd_free_return(old_pwd, 0));
	if (cd_access(home))
		return (cd_free(home, old_pwd));
	if (chdir(home) != 0)
		return (perror_free_return("chdir", home, old_pwd));
	if (ft_setenv("OLDPWD", old_pwd, &shell->env, 1))
		return (cd_free(home, old_pwd));
	if (ft_setenv("PWD", home, &shell->env, 1))
		return (cd_free(home, old_pwd));
	free(old_pwd);
	free(home);
	return (0);
}

static char	*set_oldpwd(t_shell *shell)
{
	int		i;
	char	*oldpwd;

	i = 0;
	oldpwd = NULL;
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], "PWD=", 4) == 0)
		{
			oldpwd = strdup(shell->env[i] + 4);
			if (!oldpwd)
				perror("minishell: malloc");
			break ;
		}
		i++;
	}
	return (oldpwd);
}

static int	cd_to_path(t_shell *shell, char *path)
{
	char	*oldpwd;
	char	*newpwd;

	if (change_directory(path))
		return (1);
	oldpwd = set_oldpwd(shell);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		perror("minishell: getcwd");
	if (oldpwd)
	{
		if (ft_setenv("OLDPWD", oldpwd, &shell->env, 1))
			return (cd_free(newpwd, oldpwd));
	}
	if (newpwd)
	{
		if (ft_setenv("PWD", newpwd, &shell->env, 1))
			return (cd_free(newpwd, oldpwd));
	}
	cd_free(newpwd, oldpwd);
	return (0);
}

int	ft_cd(t_shell *shell, t_ast *node)
{
	if (node->args[1] != NULL)
	{
		if (node->args[2] != NULL)
		{
			report_error(NULL, "cd: too many arguments\n");
			return (1);
		}
		if (ft_strcmp(node->args[1], "") == 0)
			return (0);
	}
	if (node->args[1] == NULL)
		return (cd_to_home(shell));
	else
		return (cd_to_path(shell, node->args[1]));
}
