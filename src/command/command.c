
#include "minishell.h"


void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "^C", 2);
	g_rl_interrupted = 2;
	rl_done = 1;
}

int	heredoc_event_hook(void)
{
	/* if (g_rl_interrupted)
	{
		rl_done = 1;
	} */
	return (0);
}

char	*append_char(char *s, char c)
{
	char str[2];
	char *result;

	str[0] = c;
	str[1] = '\0';
	result = ft_strjoin(s, str);
	free(s);
	return (result);
}

char	*ft_strjoin_free(char *a, char *b)
{
	char *tmp = ft_strjoin(a, b);
	free(a);
	return (tmp);
}

char	*expand_dollar(char *line, int *i, t_shell *shell)
{
	char *result = ft_strdup("");
	char *temp;
	char *var_name;
	char *env_value;
	char *status_str;

	if (line[*i + 1] == '?')
	{
		status_str = ft_itoa(shell->status_last_command);
		result = ft_strjoin_free(result, status_str);
		free(status_str);
		*i += 2;
	}
	else
	{
		(*i)++;
		int start = *i;
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		var_name = ft_substr(line, start, *i - start);
		env_value = get_env_value(shell->env, var_name);
		if (!env_value)
			env_value = "";
		temp = result;
		result = ft_strjoin(result, env_value);
		free(temp);
		free(var_name);
	}
	return (result);
}

char	*check_expand_heredoc(char *line, t_shell *shell)
{
	int i = 0;
	char *expanded = ft_strdup("");
	char *temp;
	char *expanded_part;

	while (line[i])
	{
		if (line[i] == '$' && (line[i + 1] == '?' || ft_isalpha(line[i + 1])
				|| line[i + 1] == '_'))
		{
			expanded_part = expand_dollar(line, &i, shell);
			temp = expanded;
			expanded = ft_strjoin(expanded, expanded_part);
			free(temp);
			free(expanded_part);
		}
		else
		{
			expanded = append_char(expanded, line[i]);
			i++;
		}
	}
	if (temp)
		free(temp);
	if(expanded_part)
		free(expanded_part);
	free(line);
	return (expanded);
}

static int	fill_heredoc(t_redirect *redir, char *name, t_shell *shell)
{
	struct sigaction old_sa;
	struct sigaction new_sa;
	char *line;
	int fd;

	new_sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&new_sa.sa_mask);
	new_sa.sa_flags = 0;
	sigaction(SIGINT, &new_sa, &old_sa);
	g_rl_interrupted = 0;
	rl_done = 0;
	rl_event_hook = heredoc_event_hook;
	fd = open(name, O_CREAT | O_RDWR | O_EXCL, 0644);
	if (fd == -1)
		return (fd);
	while (1)
	{
		if (g_rl_interrupted)
			break ;
		line = readline("> ");
		if (!line || g_rl_interrupted)
		{
			sigaction(SIGINT, &old_sa, NULL);
			break ;
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			sigaction(SIGINT, &old_sa, NULL);
			break ;
		}
		if (ft_strchr(line, '$'))
			line = check_expand_heredoc(line, shell);
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	rl_done = 0;
	rl_event_hook = NULL;
	sigaction(SIGINT, &old_sa, NULL);
	close(fd);
	if (g_rl_interrupted == 2)
	{
		shell->status_last_command = 130;
		unlink(name);
		return (-1);
	}
	fd = open(name, O_RDWR, 0640);
	unlink(name);
	printf("fd %d\n",fd);
	return (fd);
}


static int	scan_heredocs(t_ast *node, t_shell *shell)
{
	t_redirect *redir;
	char temp_name[25];
	char *num_str;
	static unsigned long i;

	i = (unsigned long)&num_str;
	redir = node->redirections;
	while (redir)
	{  
		if (redir->type == NODE_HEREDOC )
		{
			i++;
			ft_strlcpy(temp_name, "tempfile_", sizeof(temp_name));
			num_str = ft_itoa((int)i);
			if (num_str)
			{
				ft_strlcat(temp_name, num_str, sizeof(temp_name));
				free(num_str);
			}
			redir->fd_heredoc = fill_heredoc(redir, temp_name, shell);
			if (redir->fd_heredoc == -1)
				return(-1);
		}
		redir = redir->next;
	}
	if (node->left)
		scan_heredocs(node->left, shell);
	if (node->right)
		scan_heredocs(node->right, shell);
	return(0);
}

static void	prescan_redirections(t_ast *node, t_shell *shell)
{
	t_redirect *redir;
	int fd;
	int flags;

	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_REDIRECT_OUT || redir->type == NODE_APPEND)
		{
			if (redir->type == NODE_REDIRECT_OUT)
				flags = O_CREAT | O_WRONLY | O_TRUNC;
			else
				flags = O_CREAT | O_WRONLY | O_APPEND;
			if (check_file_access_write(redir->file, 3, shell))
				break ;
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
        break ;
      close(fd);
		}
		redir = redir->next;
	}
	if (node->left)
		prescan_redirections(node->left, shell);
	if (node->right)
		prescan_redirections(node->right, shell);
}

void	redir_close(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup2(out_fd, STDOUT_FILENO);
}


static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
	if (node->type == NODE_PIPE)
	{
		// rec_index++;
		if (pipe(shell->pipes[shell->pipe_index]) == -1)
		{
			perror("minishell: pipe");
			return ;
		}
		shell->pipe_index++;
		execute_ast(shell, node->left, in_fd, shell->pipes[shell->pipe_index
			- 1][1]);
		shell->index++;
		execute_ast(shell, node->right, shell->pipes[shell->pipe_index
			- shell->index][0], out_fd);
	}
	else if (node->type == NODE_COMMAND)
		execute_command(shell, node, in_fd, out_fd);
}

static void	wait_for_children(t_shell *shell)
{
	int i;
	int status;

	i = 0;
	while (i < shell->pid_index)
	{
		waitpid(shell->pid[i], &status, 0);
		if (WIFEXITED(status))
			shell->status_last_command = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) /*&& check_if_builtin(node) == 0*/)
		{
			shell->status_last_command = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				printf("\n");
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
		}
		i++;
	}
}

void	unlink_heredoc_fd(t_ast *node)
{
	t_redirect *redir;
	
	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_HEREDOC)
			close(node->redirections->fd_heredoc);
		redir = redir->next;
	}
	if (node->left)
		unlink_heredoc_fd(node->left);
	if (node->right)
		unlink_heredoc_fd(node->right);
}

void	execute_pipeline(t_shell *shell)
{
	int in_fd;
	int out_fd;

	if (scan_heredocs(shell->node, shell) == -1)
	{
		g_rl_interrupted = 0;
		return ;
	}
	shell->pid = ft_calloc(1, sizeof(pid_t) * (shell->pipe_count + 1));
	if (!shell->pid)
	{
		perror("minishell: malloc");
		shell->status_last_command = 1;
		return ;
	}
	shell->pid_index = 0;
	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	//signal(SIGINT, SIG_IGN); // not sure if correctly placed
	
	if (shell->pipe_count > 0)
	{
		shell->pipes = malloc(sizeof(int *) * shell->pipe_count);
		if (!shell->pipes)
		{
			perror("minishell: malloc");
			shell->status_last_command = 1;
			return ;
		}
		shell->pipe_index = 0;
		shell->index = 0;
		int i = 0;
		while (i < shell->pipe_count)
		{
			shell->pipes[i] = malloc(sizeof(int) * 2);
			if (!shell->pipes[i])
			{
				perror("minishell: malloc");
				while (--i >= 0)
					free(shell->pipes[i]);
				free(shell->pipes);
				shell->pipes = NULL;
				shell->status_last_command = 1;
				return ;
			}
			i++;
		}
	}
	prescan_redirections(shell->node, shell);
	if (!shell->node->cmd && shell->node->type == NODE_COMMAND)
		return ;
	else
		execute_ast(shell, shell->node, in_fd, out_fd);
	int i = 0;
	while (i < (shell->pipe_count))
	{
		close(shell->pipes[i][0]);
		close(shell->pipes[i][1]);
		i++;
	}
	wait_for_children(shell);
	unlink_heredoc_fd(shell->node);
	setup_signal_handlers();
}
