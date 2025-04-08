#include "minishell.h"

char			*find_expanded_value(t_lexer *lexer, char *token_value);
t_token			*create_delim_token(t_lexer *lexer);

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("Failed to allocate token");
		return (NULL);
	}
	token->type = type;
	if (value)
	{
		token->value = ft_strdup(value);
		if (!token->value)
			return (NULL);
	}
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}


t_token	*split_expanded_value(t_token *head, char *token_value)
{
	char	*saveptr;
	t_token *tail;
	t_token *new_word_token;
	char 	*word;

	tail = NULL;
	new_word_token = NULL;
	word = ft_strtok_r(token_value, " ", &saveptr);
	while (word) {
		new_word_token = new_token(TOKEN_WORD, word);
		if (!new_word_token || !new_word_token->value)
			return new_token(TOKEN_ERROR, "Memory allocation error");
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

// static t_token	*inner_loop(int *pos, char *quote_char, char **token_value,
// 	t_lexer *lexer)
// {
// 	char *before;
// 	char *after;

// 	while (lexer->input[*pos])
// 	{
// 		if ((lexer->input[*pos] == '"' || lexer->input[*pos] == '\'')
// 				&& (*quote_char == lexer->input[*pos] || *quote_char == 0))
// 		{
// 			if (*quote_char == lexer->input[*pos])
// 			{
// 				if (*token_value == NULL)
// 					*token_value = ft_strdup("");
// 				*quote_char = 0;
// 			}
// 			else if (*quote_char == 0)
// 			{
// 				*quote_char = lexer->input[*pos];
// 				lexer->was_quoted = 1;
// 			}
// 			(*pos)++;
// 			continue ;
// 		}
// 		else if (lexer->input[*pos] == '$' && *quote_char != '\''
// 			&& lexer->hereflag != '<')
// 		{
// 			before = *token_value;
// 			after = find_expanded_value(lexer, before);

// 			if (after)
// 				*token_value = after;
// 			//*token_value = find_expanded_value(lexer, *token_value);
// 			lexer->was_expanded = 1;
// 			//printf("{%p}", *token_value);
// 			//printf("----expanded token = %s ----\n", *token_value);
// 			/* if (!*token_value)
// 			{
// 				//return (new_token(TOKEN_ERROR, "Variable expansion error"));
// 				return (NULL);
// 			} */
// 			continue ;
// 		}
// 		else if (!*quote_char && (ft_isspace(lexer->input[*pos])
// 				|| !is_delimiter(lexer->input[*pos])))
// 			break ;
// 		if (*token_value != NULL)
// 			*token_value = ft_strncat(*token_value, &lexer->input[*pos], 1);
// 		(*pos)++;
// 	}
// 	return (NULL);
// }

static void lexer_quotes(char *quote_char, t_lexer *lexer, int *pos, char **token_value)
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

static void lexer_expander(t_lexer *lexer, char **token_value)
{
	char *before;
	char *after;

	before = *token_value;
	after = find_expanded_value(lexer, before);
	if (after)
		*token_value = after;
	lexer->was_expanded = 1;
}

static t_token	*inner_loop(int *pos, char *quote_char, char **token_value,
	t_lexer *lexer)
{
	while (lexer->input[*pos])
	{
		if ((lexer->input[*pos] == '"' || lexer->input[*pos] == '\'')
				&& (*quote_char == lexer->input[*pos] || *quote_char == 0))
		{
			lexer_quotes(quote_char, lexer, pos, token_value);
			continue ;
		}
		else if (lexer->input[*pos] == '$' && *quote_char != '\''
			&& lexer->hereflag != '<')
		{
			lexer_expander(lexer, token_value);
			continue ;
		}
		else if (!*quote_char && (ft_isspace(lexer->input[*pos])
				|| !is_delimiter(lexer->input[*pos])))
			break ;
		if (*token_value != NULL)
			*token_value = ft_strncat(*token_value, &lexer->input[*pos], 1);
		(*pos)++;
	}
	return (NULL);
}

t_token	*lexer_next_token(t_lexer *lexer, t_token *temp_token, char quote_char,
	char *token_value)
{
	t_token *head;

	head = NULL;
	lexer->was_quoted = 0;
	lexer->was_expanded = 0;
	while (lexer->input[lexer->pos])
	{
		if (!quote_char && ft_isspace(lexer->input[lexer->pos]))
		{
			lexer->pos++;
			continue ;
		}
		if (!quote_char)
		{
			temp_token = create_delim_token(lexer);
			if (temp_token)
				return (temp_token);
		}
		token_value = ft_calloc(1, ft_strlen(lexer->input) + 1);
		if (!token_value)
			return (NULL);
		temp_token = inner_loop((int *)&lexer->pos, &quote_char, &token_value, lexer);
		if (temp_token)
			return (free(token_value), temp_token);
		/* if(!temp_token)
			continue;; */
		if (quote_char)
			return (free(token_value), new_token(TOKEN_ERROR, "Unclosed quote"));
		if (token_value == NULL)
			continue;
		if (lexer->was_quoted == 0 && lexer->was_expanded == 1)
			head = split_expanded_value(head, token_value);
		else
			head = new_token(TOKEN_WORD, token_value);
		free(token_value);
		if(head)
			return (head);
		else
			return new_token(TOKEN_WORD,(""));
	}
	return (new_token(TOKEN_EOF, NULL));
}
