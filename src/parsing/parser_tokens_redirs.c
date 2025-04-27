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
