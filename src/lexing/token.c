#include "minishell.h"

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

t_token	*lexer_try_delim_token(t_lexer *lexer, char quote_char)
{
	t_token	*temp_token;

	temp_token = NULL;
	if (!quote_char)
	{
		temp_token = create_delim_token(lexer);
		if (temp_token)
			return (temp_token);
	}
	return (NULL);
}

t_token	*lexer_process_token_value(t_lexer *lexer, char *token_value)
{
	t_token	*head;

	head = NULL;
	if (lexer->was_quoted == 0 && lexer->was_expanded == 1)
		head = split_expanded_value(head, token_value, NULL);
	else
		head = new_token(TOKEN_WORD, token_value);
	if (head)
		return (head);
	else
		return (new_token(TOKEN_WORD, ""));
}
