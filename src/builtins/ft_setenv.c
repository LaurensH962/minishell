#include "minishell.h"

//Updates enviroment variables or creates them if they do not already exist

static int search_for_key(const char *key, char ***envp, char *new_entry)
{
    int i;
    int key_len;
   
    i = 0;
    key_len = ft_strlen(key);
    if (*envp)
	{
		while ((*envp)[i])
		{
			if (ft_strncmp((*envp)[i], key, key_len) == 0 /*&& (*envp)[i][key_len] == '='*/)
			{
				free((*envp)[i]);
				(*envp)[i] = new_entry;
				return (1);
			}
			i++;
		}
	}
    return (0);
}

static int add_new_entry(char ***envp, char *new_entry)
{
    char **new_env;
    int i;

    i = 0;
    while (*envp && (*envp)[i])
		i++;
    new_env = malloc((i + 2) * sizeof(char *));
    if (!new_env)
        return(perror_malloc_return());
    i = 0;
	while (*envp && (*envp)[i])
    {
        new_env[i] = strdup((*envp)[i]);  // Use strdup to allocate and copy strings
        if (!new_env[i])  // Check if strdup failed
        {
            free_array(new_env, i);
            return (perror_malloc_return());
        }
        i++;
    }
    new_env[i++] = new_entry;  // Add the new key=value pair
    new_env[i] = NULL;
    free_array((*envp), -1);
    *envp = new_env;
    return (0);
}

int ft_setenv(const char *key, const char *value, char ***envp)
{   
    char *new_entry;

    new_entry = ft_strjoin(key, value);
    if (!new_entry)
        return (1);
    if (search_for_key(key, envp, new_entry))
        return (0);
    else
    {
        if (add_new_entry(envp, new_entry))
        {
            free(new_entry);
            return (1);
        }
        return (0);
    }
    
}
