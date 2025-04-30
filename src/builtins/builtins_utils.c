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

void	print_export_if_equalsign(t_shell *shell, int *index_string, int *index_char)
{
	printf("=\"");
	(*index_char)++;
	while (shell->export[*index_string][*index_char])
	{
		printf("%c", shell->export[*index_string][*index_char]);
		if (shell->export[*index_string][(*index_char) + 1] == '\0')
			printf("\"\n");
		(*index_char)++;
	}
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
