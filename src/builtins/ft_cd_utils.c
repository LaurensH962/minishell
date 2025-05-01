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