#include "minishell.h"

// copies the envp from main
int		copy_environ(char **envp, char ***env)
{
	int		count;
	int		i;

	count = 0;
	while(envp[count] != NULL)
		count++;
	if (count == 0)
		return (1);
	*env = malloc(sizeof(char *) * (count + 1));
	if (*env == NULL)
		return(perror_malloc_return());
	i = 0;
	while(i < count)
	{
		(*env)[i] = ft_strdup(envp[i]);
		if (!(*env)[i])
		{
			free_array(*env, i);
			perror("minishell: malloc");
			return (1);
		}
		i++;
	}
	(*env)[i] = NULL;
	return (0);
}

//Helper function for export_default_variables
static void	set_key_value(char **key, char **value, char *env, char* key_value)
{
	*value = strdup(env + 4);
	*key =	key_value;
}

//Helper function for export_default_variables
static void	set_variable(t_shell *shell, char *key, char *value)
{
	ft_setenv(key, value, &(shell->export));
	free(value);
}

//Creates the array for exported enviroment variables and sets default variables
void	export_default_variables(t_shell *shell)
{
	int i;
	char *value;
	char *key;

	i = 0;
	value = NULL;
	key = NULL;
	shell->export = NULL;
	while(shell->env[i] != NULL)
	{
		printf("test1\n");
		if(ft_strncmp(shell->env[i], "PATH=", 5) == 0)
			set_key_value(&key, &value, shell->env[i], "PATH");
		if(ft_strncmp(shell->env[i], "HOME=", 5) == 0)
			set_key_value(&key, &value, shell->env[i], "HOME");
		if(ft_strncmp(shell->env[i], "PWD=", 4) == 0)
			set_key_value(&key, &value, shell->env[i], "PWD");
		if(ft_strncmp(shell->env[i], "SHELL=", 6) == 0)
			set_key_value(&key, &value, shell->env[i], "SHELL");
		if(ft_strncmp(shell->env[i], "OLDPWD=", 7) == 0)
			set_key_value(&key, &value, shell->env[i], "OLDPWD");
		if(key && value)
			set_variable(shell, key, value);
		i++;
	}
}

//Updates enviroment variables or creates them if they do not already exist

