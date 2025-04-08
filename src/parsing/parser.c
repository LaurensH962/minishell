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

t_ast	*parse_redirection(t_token **tokens, t_ast *command,
		t_token *redirection_token, t_token *filename_token)
{
	t_redirect	*redir;
	t_redirect	**redir_ptr;

	redir_ptr = &command->redirections;
	while (*tokens && is_redirect((*tokens)->type))
	{
		redirection_token = get_next_token(tokens);
		filename_token = get_next_token(tokens);
		if (!filename_token || filename_token->type != TOKEN_WORD)
			return (NULL);
		redir = ft_calloc(1, sizeof(t_redirect));
		redir->file = ft_strdup(filename_token->value);
		redir->next = NULL;
		if (redirection_token->type == TOKEN_REDIRECT_OUT)
			redir->type = NODE_REDIRECT_OUT;
		else if (redirection_token->type == TOKEN_REDIRECT_IN)
			redir->type = NODE_REDIRECT_IN;
		else if (redirection_token->type == TOKEN_APPEND)
			redir->type = NODE_APPEND;
		else if (redirection_token->type == TOKEN_HEREDOC)
			redir->type = NODE_HEREDOC;
		while (*redir_ptr)
			redir_ptr = &(*redir_ptr)->next;
		*redir_ptr = redir;
	}
	return (command);
}

t_ast	*parse_command(t_token **tokens, t_token *token, int arg_count,
		int args_capacity)
{
	t_ast	*command_node;
	char	**args;

	command_node = ft_calloc(1, sizeof(t_ast));
	command_node->type = NODE_COMMAND;
	command_node->cmd_path = NULL;
	command_node = parse_redirection(tokens, command_node, NULL, NULL);
	if (*tokens == NULL || (*tokens)->type != TOKEN_WORD || !command_node)
		return (command_node);
	token = get_next_token(tokens);
	command_node->cmd = ft_strdup(token->value);
	args = ft_calloc(sizeof(char *), args_capacity);
	args[arg_count++] = ft_strdup(token->value);
	while (*tokens)
	{
		while (*tokens && is_redirect((*tokens)->type))
			command_node = parse_redirection(tokens, command_node, NULL, NULL);
		if (*tokens == NULL || (*tokens)->type != TOKEN_WORD)
			break ;
		if (arg_count >= args_capacity - 1)
		{
			args_capacity *= 2;
			args = ft_realloc(args, sizeof(char *) * args_capacity / 2,
					sizeof(char *) * args_capacity);
		}
		token = get_next_token(tokens);
		args[arg_count++] = ft_strdup(token->value);
	}
	args[arg_count] = NULL;
	command_node->args = args;
	return (command_node);
}

t_ast	*parse_pipeline(t_token **tokens, t_ast *left_command,
		t_ast *right_command, t_ast *pipe_node)
{
	left_command = parse_command(tokens, NULL, 0, 2);
	if (!left_command)
		return (NULL);
	left_command = parse_redirection(tokens, left_command, NULL, NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		get_next_token(tokens);
		if (!*tokens || (*tokens)->type == TOKEN_PIPE)
			return (NULL);
		right_command = parse_command(tokens, NULL, 0, 2);
		if (!right_command)
			return (NULL);
		right_command = parse_redirection(tokens, right_command, NULL, NULL);
		if (!right_command)
			return (NULL);
		pipe_node = ft_calloc(1, sizeof(t_ast));
		if (!pipe_node)
			return (NULL);
		pipe_node->type = NODE_PIPE;
		pipe_node->left = left_command;
		pipe_node->right = right_command;
		pipe_node->cmd_path = NULL;
		left_command = pipe_node;
	}
	return (left_command);
}

t_ast	*parse(t_token *tokens)
{
	t_ast	*tree;

	tree = parse_pipeline(&tokens, NULL, NULL, NULL);
	if (!tree)
		return(NULL);
	return (parse_redirection(&tokens, tree, NULL, NULL));
}
