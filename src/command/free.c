#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i] != NULL)
		free(split[i++]);
	free(split);
}

void free_array(char **array, int len)
{
	int i;

	i = 0;
    if (!array)
        return ;
    while(array[i] && (len == -1 || i < len))
		free(array[i++]);
    free(array);
}
