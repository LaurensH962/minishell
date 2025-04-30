#include "minishell.h"


void	cleanup_redirect(t_redirect *redirect)
{
	t_redirect *tmp;

	while (redirect)
	{
		tmp = redirect;
		if (tmp->type == NODE_HEREDOC)
		{	
			if (tmp->fd_heredoc > 2)
				close(tmp->fd_heredoc);
		}
		redirect = redirect->next;
		free(tmp->file); // Free the file associated with the redirect
		free(tmp);       // Free the redirect structure itself
	}
}

void	cleanup_ast(t_ast **node)
{
	int i;

	if ((*node) == NULL)
		return ;
	cleanup_ast(&(*node)->left);             // Recursively clean up left child
	cleanup_ast(&(*node)->right);           
		// Recursively clean up right child
	cleanup_redirect((*node)->redirections); // Clean up redirections
	free((*node)->cmd);                      // Free command string
	free((*node)->cmd_path);                 // Free command path string
	if ((*node)->args)
	{
		i = 0;
		while ((*node)->args[i]) // Free arguments if they exist
			free((*node)->args[i++]);
		free((*node)->args); // Free the argument array itself
	}
	free((*node)); // Finally, free the node itself
	(*node) = NULL;
}

void	cleanup_pipes_pids(t_shell *shell)
{
	int i = 0;

	if (shell->pipe_count)
	{
		while (i < shell->pipe_count)
			free(shell->pipes[i++]);
		free(shell->pipes);
	}
	if (shell->pid)
		free(shell->pid);
}

void	cleanup_shell(t_shell *shell)
{
	int i;

	if (shell == NULL)
		return ;
	// free_structs(shell);    // Clean up AST nodes
	if (shell->env)
	{
		i = 0;
		while (shell->env[i]) // Free environment variables
			free(shell->env[i++]);
		free(shell->env); // Free environment array
	}
	if (shell->export)
	{
		i = 0;
		while (shell->export[i]) // Free environment variables
			free(shell->export[i++]);
		free(shell->export); // Free environment array
	}
	free(shell);
}
