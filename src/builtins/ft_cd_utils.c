#include "minishell.h"

/*int		check_pwd_exits(t_shell *shell)
{
	int	i;

	i = 0;
	if(shell->env)
	{
		while(shell->env[i] != NULL)
		{
			if (ft_strncmp(shell->env[i], "PWD", 3) == 0)
				return (1);
		}
	}
	return (0);
}

int check_oldpwd_exits(t_shell *shell)
{
	int	i;

	i = 0;
	if(shell->env)
	{
		while(shell->env[i] != NULL)
		{
			if (ft_strncmp(shell->env[i], "OLDPWD", 6) == 0)
				return (1);
		}
	}
	return (0);
}*/

int	change_directory(char *path)
{
	if (cd_access(path))
		return (1);
	if (chdir(path) != 0)
		return(perror_cd_return());
	return (0);
}


int cd_free(char *new_pwd, char *old_pwd)
{
	if(new_pwd)
		free(new_pwd);
	if (old_pwd)
		free(old_pwd);
	return (1);
}

static int    cd_free_return(char *string)
{
    if (string)
        free(string);
    return (perror_malloc_return());
}


int     set_home_oldpwd(t_shell *shell, char **home, char **old_pwd)
{
	int	i;

	i = 0;
    while (shell->env[i]) 
	{
		if (ft_strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			*home = ft_strdup(shell->env[i] + 5);
			if (!(*home))
                return (cd_free_return(*old_pwd));
		}
		if (ft_strncmp(shell->env[i], "PWD=", 4) == 0)
		{
			*old_pwd = ft_strdup(shell->env[i] + 4);
			if (!(*old_pwd))
                return (cd_free_return(*home));
		}
		i++;
	}
    return (0);
}