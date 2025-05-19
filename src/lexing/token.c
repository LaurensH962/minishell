/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:18:40 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/19 16:18:40 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		{
			free (token);
			perror("Failed to duplicate token value");
			return (NULL);
		}
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
