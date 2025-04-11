#include "minishell.h"

//Updates enviroment variables or creates them if they do not already exist
int ft_setenv(const char *key, const char *value, char ***envp)
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
}