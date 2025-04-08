#include "minishell.h"
#include <stdio.h>

void	print_ast(t_ast *node, int depth)
{
	t_redirect	*redir;

	if (!node)
		return ;
	// Indentation for readability
	for (int i = 0; i < depth; i++)
		printf("  ");
	// Print node type
	if (node->type == NODE_COMMAND)
	{
		printf("COMMAND: {%s}\n", node->cmd ? node->cmd : "(empty)");
		printf("COMMAND PATH: {%s}\n", node->cmd_path ? node->cmd_path : "(empty)");
		// Print arguments (including args[0])
		if (node->args)
		{
			for (int i = 0; node->args[i]; i++)
			{
				for (int j = 0; j < depth + 1; j++)
					printf("  ");
				printf("ARG: {%s}\n", node->args[i]);
			}
		}
	}
	else if (node->type == NODE_PIPE)
	{
		printf("PIPE\n");
	}
	// Print redirections (for all nodes, not just commands)
	redir = node->redirections;
	while (redir)
	{
		for (int j = 0; j < depth + 1; j++)
			printf("  ");
		if (redir->type == NODE_REDIRECT_IN)
			printf("REDIR: < {%s}\n", redir->file);
		else if (redir->type == NODE_REDIRECT_OUT)
			printf("REDIR: > {%s}\n", redir->file);
		else if (redir->type == NODE_APPEND)
			printf("REDIR: >> {%s}\n", redir->file);
		else if (redir->type == NODE_HEREDOC)
			printf("REDIR: << {%s}\n", redir->file);
		redir = redir->next;
	}
	// Recursive call for left and right children
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token Type: {%d}, Value: {%s}\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}
