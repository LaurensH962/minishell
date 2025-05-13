#include "minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

// deletes enviroment variables from the environment array
static void	remove_variable(char ***env, const char *var)
{
	int	i;
	int	j;

	i = 0;
	if (!env || !*env)
		return ;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], var, ft_strlen(var)) == 0
			&&((*env)[i][ft_strlen(var)] == '=' || (*env)[i][ft_strlen(var)] == '\0'))
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j + 1] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			(*env)[j] = NULL;
			return ;
		}
		i++;
	}
}

int	ft_unset(t_shell *shell, char **vars)
{
	int i;

	if (!shell || !vars || !vars[0])
		return (0);
	i = 0;
	while (vars[i])
	{
		remove_variable(&(shell->env), vars[i]);
		remove_variable(&(shell->export), vars[i]);
		i++;
	}
	return (0);
}

int	ft_env(t_shell *shell, t_ast *node)
{
	int	i;

	i = 0;
	if (shell->env == NULL)
		return (1);
	if (node->args[1] != NULL)
	{
		printf("minishell: env: %s: No such file or directory\n",
			node->args[1]);
		return (1);
	}
	while (shell->env[i] != NULL)
	{
		if (strchr(shell->env[i], '=') != NULL)
			printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}
