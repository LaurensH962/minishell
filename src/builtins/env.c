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
