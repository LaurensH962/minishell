#include "minishell.h"

char	*build_error_msg(const char *unexpected)
{
	char	*prefix;
	char	*suffix;
	char	*temp;
	char	*result;

	prefix = "syntax error near unexpected token `";
	suffix = "'";
	temp = ft_strjoin(prefix, unexpected);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, suffix);
	free(temp);
	return (result);
}

char	*syntax_checker(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!prev || tokens->next->type == TOKEN_EOF || tokens->next->type == TOKEN_PIPE)
				return (build_error_msg("|"));
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_APPEND
			|| tokens->type == TOKEN_HEREDOC)
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
				return (build_error_msg("newline"));
		}
		else if (tokens->type == TOKEN_ERROR)
			return (build_error_msg(tokens->value));
		prev = tokens;
		tokens = tokens->next;
	}
	return (NULL);
}
