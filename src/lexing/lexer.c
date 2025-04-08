#include "minishell.h"

#define EMPTY_STRING ""

char	*get_env_value(char **env, const char *var_name)
{
	int		i;
	size_t	len;

	if (!env || !var_name)
		return (NULL);
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_variable(const char *input, size_t *pos, t_lexer *lexer)
{
	size_t	start;
	char	var_name[256];
	char	*expanded_value;
	char	*env_value;
	size_t	i;

	start = *pos + 1;
	if (input[start] == '?')
	{
		expanded_value = ft_itoa(lexer->shell->status_last_command);
		*pos = start + 1;
		return (expanded_value);
	}
	if (*input == '\0' || ft_isspace(input[start]))
	{
		(*pos)++;
		return ("$");
	}
	i = 0;
	while (input[start] && (ft_isalnum(input[start]) || input[start] == '_')
		&& i < 255)
		var_name[i++] = input[start++];
	var_name[i] = '\0';
	if (i == 0)
	{
		(*pos)++;
		return ("$");
	}
	env_value = get_env_value(lexer->shell->env, var_name);
	if (env_value)
		expanded_value = ft_strdup(env_value);
	else
	{
		*pos = start;
		return (NULL);
	}
	*pos = start;
	return (expanded_value);
}

char	*find_expanded_value(t_lexer *lexer, char *token_value)
{
	char	*expanded_value;

	expanded_value = expand_variable(lexer->input, &(lexer->pos), lexer);
	/* if (!expanded_value)
		return (NULL); */
	token_value = ft_strjoin_minishell(token_value, expanded_value, lexer);
	if (expanded_value && !ft_strcmp(expanded_value, "$") && !ft_strcmp (expanded_value, ""))
        free(expanded_value);
	return (token_value);
}

t_token	*create_delim_token(t_lexer *lexer)
{
	if (lexer->input[lexer->pos] == '|')
		return (lexer->pos++, new_token(TOKEN_PIPE, "|"));
	if (lexer->input[lexer->pos] == '>')
	{
		if (lexer->input[lexer->pos + 1] == '>')
			return (lexer->pos += 2, new_token(TOKEN_APPEND, ">>"));
		return (lexer->pos++, new_token(TOKEN_REDIRECT_OUT, ">"));
	}
	if (lexer->input[lexer->pos] == '<')
	{
		if (lexer->input[lexer->pos + 1] == '<')
		{
			lexer->hereflag = '<';
			return (lexer->pos += 2, new_token(TOKEN_HEREDOC, "<<"));
		}
		return (lexer->pos++, new_token(TOKEN_REDIRECT_IN, "<"));
	}
	else
		return (NULL);
}

void	init_lexer(t_token **tokens, t_lexer *lexer, t_shell *shell, char *line)
{
	*tokens = NULL;
	lexer->input = line;
	lexer->pos = 0;
	lexer->shell = shell;
	lexer->hereflag = '\0';
}

t_token	*lexer(char *line, t_shell *shell)
{
	t_lexer	lexer;
	t_token	*tokens;
	t_token	*current_token;

	current_token = NULL;
	init_lexer(&tokens, &lexer, shell, line);
	current_token = lexer_next_token(&lexer, NULL, '\0', NULL);
	while (current_token->type != TOKEN_EOF)
	{
		if (current_token->type == TOKEN_ERROR)
		{
			fprintf(stderr, "Lexer error: %s\n", current_token->value);
			free_structs(shell);
			free(current_token);
			return (NULL);
		}
		add_token(&tokens, current_token);
		current_token = lexer_next_token(&lexer, NULL, '\0', NULL);
		if (!current_token)
			return(NULL);
	}
	add_token(&tokens, current_token);
	return (tokens);
}
