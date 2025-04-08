#include "minishell.h"

static char	*find_expanded_value(t_lexer *lexer, char *token_value)
{
	char	*expanded_value;

	expanded_value = expand_variable(lexer->input, &(lexer->pos), lexer);
	/* if (!expanded_value)
		return (NULL); */
	token_value = ft_strjoin_minishell(token_value, expanded_value, lexer);
	if (expanded_value && !ft_strcmp(expanded_value, "$")
		&& !ft_strcmp(expanded_value, ""))
		free(expanded_value);
	return (token_value);
}

void	lexer_expander(t_lexer *lexer, char **token_value)
{
	char	*before;
	char	*after;

	before = *token_value;
	after = find_expanded_value(lexer, before);
	if (after)
		*token_value = after;
	else
		*token_value = NULL;
	lexer->was_expanded = 1;
}

t_token	*split_expanded_value(t_token *head, char *token_value, char *word)
{
	char	*saveptr;
	t_token	*tail;
	t_token	*new_word_token;

	tail = NULL;
	new_word_token = NULL;
	word = ft_strtok_r(token_value, " ", &saveptr);
	while (word)
	{
		new_word_token = new_token(TOKEN_WORD, word);
		if (!new_word_token || !new_word_token->value)
			return (new_token(TOKEN_ERROR, "Memory allocation error"));
		if (!head)
		{
			head = new_word_token;
			tail = new_word_token;
		}
		else
		{
			tail->next = new_word_token;
			tail = new_word_token;
		}
		word = ft_strtok_r(NULL, " ", &saveptr);
	}
	return (head);
}

char	*expand_variable(const char *input, size_t *pos, t_lexer *lexer)
{
	char	var_name[256];
	char	*expanded_value;
	size_t	start;

	start = *pos + 1;
	expanded_value = handle_special_cases(input, pos, lexer);
	if (expanded_value)
		return (expanded_value);
	if (!extract_var_name(input, &start, var_name))
	{
		(*pos)++;
		return (ft_strdup("$"));
	}
	expanded_value = lookup_and_expand(var_name, lexer);
	if (!expanded_value)
	{
		*pos = start;
		return (NULL);
	}
	*pos = start;
	return (expanded_value);
}
