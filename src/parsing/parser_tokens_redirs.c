#include "minishell.h"

t_token	*get_next_token(t_token **tokens)
{
	t_token	*current;

	if (*tokens == NULL)
		return (NULL);
	current = *tokens;
	*tokens = (*tokens)->next;
	return (current);
}

char	**resize_args(char **args, int *args_capacity)
{
	int	old_capacity;

	old_capacity = *args_capacity;
	*args_capacity *= 2;
	return (ft_realloc(args, sizeof(char *) * old_capacity, sizeof(char *)
			* (*args_capacity)));
}

t_token	*skip_invalid_node(t_token *token, t_token **tokens)
{
	while ((*tokens)->type == TOKEN_INVALID)
		token = get_next_token(tokens);
	return (token);
}

void	fill_redir_type(t_redirect *redir, t_token *redirection_token)
{
	if (redirection_token->type == TOKEN_REDIRECT_OUT)
		redir->type = NODE_REDIRECT_OUT;
	else if (redirection_token->type == TOKEN_REDIRECT_IN)
		redir->type = NODE_REDIRECT_IN;
	else if (redirection_token->type == TOKEN_APPEND)
		redir->type = NODE_APPEND;
	else if (redirection_token->type == TOKEN_HEREDOC)
		redir->type = NODE_HEREDOC;
}

t_ast	*parse_redirection(t_token **tokens, t_ast *command,
		t_token *redirection_token, t_token *filename_token)
{
	t_redirect	*redir;
	t_redirect	**redir_ptr;

	redir_ptr = &command->redirections;
	if ((*tokens)->type == TOKEN_INVALID)
		return (command);
	while (*tokens && is_redirect((*tokens)->type))
	{
		redirection_token = get_next_token(tokens);
		filename_token = get_next_token(tokens);
		if (!filename_token || filename_token->type != TOKEN_WORD)
			return (NULL);
		redir = ft_calloc(1, sizeof(t_redirect));
		redir->file = ft_strdup(filename_token->value);
		redir->next = NULL;
		fill_redir_type(redir, redirection_token);
		while (*redir_ptr)
			redir_ptr = &(*redir_ptr)->next;
		*redir_ptr = redir;
	}
	return (command);
}
