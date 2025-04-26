#include "minishell.h"

int is_valid_identifier(const char *str)
{
    int i;

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

/*int ft_export(t_shell *shell, char **args)
{
    int i;
	char *equal;
	char *key;
	char *value;

	i = 0;
    if (!args[1]) // No arguments: print exported variables
    {
        while(shell->export[i] != NULL)
		{
			printf("%s\n", shell->export[i]);
			i++;
		}
		return (0);
    }
	i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
		{
            ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
        else
        {
            equal = ft_strchr(args[i], '=');
            if (equal) // VAR=VALUE case
            {
                key = ft_substr(args[i], 0, (equal + 1) - args[i]);
                value = strdup(equal + 1);
				if (!value || !key)
					return (perror_malloc_return());
                if (ft_setenv(key, value, &(shell->export)))
					return (perror_malloc_free_return(key, value));
                if (ft_setenv(key, value, &(shell->env)))
					return (perror_malloc_free_return(key, value));
				free(key);
				free(value);
            }
			else
            {
            	if (ft_setenv(args[i], NULL, &(shell->export)) != 0)
					return (perror_malloc_return());
            }
        }
        i++;
    }
	return (0);
}*/

// deletes enviroment variables from the environment array
static void remove_variable(char ***env, const char *var)
{
    int i;
	int j;

	i = 0;
	if (!env || !*env)
        return ;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], var, ft_strlen(var)) == 0 && (*env)[i][ft_strlen(var)] == '=')
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j + 1] != NULL) 	// Shift remaining variables left
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

int ft_unset(t_shell *shell, char **vars) //needs arguments //does not remove variable
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

int ft_env(t_shell *shell, t_ast *node)
{
    int i = 0;

	if (shell->env == NULL)
		return (1);
	if (node->args[1] != NULL)
	{
		printf("minishell: env: %s: No such file or directory\n", node->args[1]);
		return (1);
	}
    while (shell->env[i] != NULL)
    {
        if (strchr(shell->env[i], '=') != NULL) // Only print variables with '='
            printf("%s\n", shell->env[i]);
        i++;
    }
	return (0);
}



/*int ft_setenv(const char *key, const char *value, char ***envp)
{
    int i = 0;
    int key_len = ft_strlen(key);
    char *new_entry = ft_strjoin(key, value);

    if (!new_entry)
        return (1);
	else if (*envp)
	{
		while ((*envp)[i])
		{
			if (ft_strncmp((*envp)[i], key, key_len) == 0 && (*envp)[i][key_len] == '=')
			{
				free((*envp)[i]);
				(*envp)[i] = new_entry;
				return (0);
			}
			i++;
		}
	}
    // If key is not found, add a new entry
    char **new_env = malloc((i + 2) * sizeof(char *));
    if (!new_env)
    {
        free(new_entry);
        return (1);
    }
    i = 0;
	while ((*envp)[i])
    {
        new_env[i] = strdup((*envp)[i]);  // Use strdup to allocate and copy strings
        if (!new_env[i])  // Check if strdup failed
        {
            // Cleanup in case of failure
            for (int j = 0; j < i; j++)
                free(new_env[j]);
            free(new_env);
            free(new_entry);
            return (1);
        }
        i++;
    }
    new_env[i++] = new_entry;  // Add the new key=value pair
    new_env[i] = NULL;
	for (i = 0; (*envp)[i]; i++)
        free((*envp)[i]);
    free(*envp);
    *envp = new_env;
    return (0);
}*/

