#include "minishell.h"

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

int perror_free_return(char *function_name, char *string)
{
	printf("minishell: %s: %s", function_name, string);
	perror(" ");
	free(string);
	return (1);
}

int	perror_malloc_free_return(char *key, char *value)
{
	perror("minishell: malloc");
	free(key);
	free(value);
	return (1);
}

char *perror_return(void)
{
	perror("minishell: malloc");
    return (NULL);
}