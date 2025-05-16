#include "minishell.h"

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

void	lexer_quotes(char *quote_char, t_lexer *lexer, int *pos,
		char **token_value)
{
	if (*quote_char == lexer->input[*pos])
	{
		if (*token_value == NULL)
			*token_value = ft_strdup("");
		*quote_char = 0;
	}
	else if (*quote_char == 0)
	{
		*quote_char = lexer->input[*pos];
		lexer->was_quoted = 1;
	}
	(*pos)++;
}

bool	lexer_skip_whitespaces(t_lexer *lexer, char quote_char)
{
	if (!quote_char && ft_isspace(lexer->input[lexer->pos]))
	{
		lexer->pos++;
		return (true);
	}
	return (false);
}

int	inner_quotes_expand(char *quote_char, t_lexer *lexer, int *pos,
		char **token_value)
{
	if ((lexer->input[*pos] == '"' || lexer->input[*pos] == '\'')
		&& (*quote_char == lexer->input[*pos] || *quote_char == 0))
	{
		lexer_quotes(quote_char, lexer, pos, token_value);
		return (-1);
	}
	else if (lexer->input[*pos] == '$' && *quote_char != '\''
		&& lexer->hereflag != '<')
	{
		lexer_expander(lexer, token_value);
		return (-1);
	}
	return (0);
}
