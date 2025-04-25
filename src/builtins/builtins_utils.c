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

int		cd_minus(t_shell *shell)
{
	char	*oldpwd;

	oldpwd = get_oldpwd(shell);
	if(oldpwd)
	{
		if (chdir(oldpwd) != 0)
			return(perror_cd_return());
	}
	printf("%s\n", oldpwd);
	return (0);
}


