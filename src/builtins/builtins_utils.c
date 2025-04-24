#include "minishell.h"

int is_number(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
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


