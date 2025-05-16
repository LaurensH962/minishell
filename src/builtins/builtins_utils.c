#include "minishell.h"

int	is_number(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	print_export_if_equalsign(t_shell *shell, int *index_string,
		int *index_char)
{
	printf("=\"");
	(*index_char)++;
	if (shell->export[*index_string][*index_char] == '\0')
		printf("\"\n");
	while (shell->export[*index_string][*index_char])
	{
		printf("%c", shell->export[*index_string][*index_char]);
		if (shell->export[*index_string][(*index_char) + 1] == '\0')
			printf("\"\n");
		(*index_char)++;
	}
}

char	*get_oldpwd(t_shell *shell, char *cmd)
{
	int		i;
	int		malloc;
	char	*oldpwd;

	i = 0;
	malloc = 0;
	oldpwd = NULL;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "OLDPWD=", 7) == 0)
		{
			oldpwd = ft_strdup(shell->env[i] + 7);
			if (!oldpwd)
			{
				perror("minishell: malloc");
				malloc = 1;
			}
			break ;
		}
		i++;
	}
	if (!oldpwd && malloc == 0)
		report_error(cmd, ": OLDPWD not set\n");
	return (oldpwd);
}

int	free_return(char *string)
{
	free(string);
	return (1);
}

char	*pwd_not_set(char *value, int *malloced)
{
	value = malloc(sizeof(char) * 1);
	if (!value)
		return (perror_return());
	*value = '\0';
	*malloced = 1;
	return (value);
}
