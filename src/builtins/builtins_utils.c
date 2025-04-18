#include "minishell.h"

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

int	perror_malloc_return(void)
{
	perror("minishell: malloc");
	return (1);
}

int	perror_cd_return(void)
{
	perror("minishell: cd");
	return (1);
}

int	perror_malloc_free_return(char * key, char *value)
{
	perror("minishell: malloc");
	free(key);
	free(value);
	return (1);
}


