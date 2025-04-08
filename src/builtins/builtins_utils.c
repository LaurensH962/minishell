#include "minishell.h"

void free_array(char **array, int len)
{
	int i;

	i = 0;
    if (!array)
        return ;
    while(array[i] && i < len)
		free(array[i++]);
    free(array);
}

int is_number(const char *str)
{
	while(*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
