#include "minishell.h"

// copies the envp from main
int	copy_environ(char **envp, char ***env)
{
	int	count;
	int	i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	if (count == 0)
		return (1);
	*env = malloc(sizeof(char *) * (count + 1));
	if (*env == NULL)
		return (perror_malloc_return());
	i = 0;
	while (i < count)
	{
		(*env)[i] = ft_strdup(envp[i]);
		if (!(*env)[i])
		{
			free_array(*env, i);
			perror("minishell: malloc");
			return (1);
		}
		i++;
	}
	(*env)[i] = NULL;
	return (0);
}

/*int		copy_pwd(char **envp, char ***env)
{
	int		count;
	int		i;
	int		j;

	j = 0;
	count = 0;
	if (!envp)
		return (1);
	while(envp[count] != NULL)
		count++;
	if (count == 0)
		return (1);
	*env = malloc(sizeof(char *) * 3);
	if (*env == NULL)
		return(perror_malloc_return());
	i = 0;
	j = 0;
	while(i < count)
	{
		if (ft_strncmp(envp[i], "PWD", 3) == 0)
		{
			(*env)[j++] = ft_strdup((envp)[i]);
			if (!(*env)[j - 1])
			{
				free_array(*env, j - 1);
				perror("minishell: malloc");
				return (1);
			}
		}
		if (ft_strncmp(envp[i], "OLDPWD", 6) == 0)
		{
			(*env)[j++] = ft_strdup((envp)[i]);
			if (!(*env)[j - 1])
			{
				free_array(*env, j - 1);
				perror("minishell: malloc");
				return (1);
			}
		}
		i++;
	}
	(*env)[j] = NULL;
	return (0);
}*/
