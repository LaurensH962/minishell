
#include "minishell.h"

int	fill_heredoc(t_redirect *redir, char *name)
{
	char	*line;
	char	tmp_path[64];
	int		fd;

	fd = open(name, O_CREAT | O_RDWR | O_EXCL, 0644);
	if (fd != -1)
		return (fd);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	unlink(tmp_path);
	return (fd);
}

static void	scan_heredocs(t_ast *node)
{
	t_redirect	*redir;
	char		temp_name[25];
	char		*num_str;
	static int	i;
    
    i = 0;

	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_HEREDOC)
		{
			i++;
			ft_strlcpy(temp_name, "tempfile_", sizeof(temp_name));
			num_str = ft_itoa(i);
			if (num_str)
			{
				ft_strlcat(temp_name, num_str, sizeof(temp_name));
				free(num_str);
			}
			redir->fd_heredoc = fill_heredoc(redir, temp_name);
		}
		redir = redir->next;
	}
	if (node->left)
        scan_heredocs(node->left);
	if (node->right)
        scan_heredocs(node->right);
}

static void prescan_redirections(t_ast *node)
{
    t_redirect *redir;
    int fd ;
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
            fd = open(redir->file, flags, 0644);
            if (fd != -1)
                close(fd);
            else
                perror("minishell: prescan");
        }
        redir = redir->next;
    }
    if (node->left)
        prescan_redirections(node->left);
    if (node->right)
        prescan_redirections(node->right);
}

static void redir_close(int in_fd, int out_fd)
{
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
}

static void     handle_redirections(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
    int fd_read;
    int fd_write;
    int heredoc_pipe[2];
    t_redirect *redir;

    redir = node->redirections;
    redir_close(in_fd, out_fd);
    while(redir)
    {
        if (redir->type == NODE_REDIRECT_IN)
            handle_inputfile(&fd_read, node);
        if (redir->type == NODE_REDIRECT_OUT)
            handle_outputfile(&fd_write, node);
        if (redir->type == NODE_APPEND)
            handle_outputfile(&fd_write, node);
        if (redir->type == NODE_HEREDOC)
            handle_heredoc(heredoc_pipe, node, shell);
        redir = redir->next;
    }
}

/*static void	set_exitstatus(t_shell *shell, t_ast *node, int status)
{
    int copy_last_command;

	if (node->args != NULL)
	{
		if(ft_strcmp(node->args[0], "exit") == 0)
		{
			shell->status_last_command = WEXITSTATUS(status);
            copy_last_command = shell->status_last_command;
			cleanup_shell(shell);
			exit(copy_last_command);
		}
	}
	shell->status_last_command = WEXITSTATUS(status);
}*/

static void   execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    pid_t pid;

    if(check_if_builtin(node) && shell->pipe_count == 0)
        shell->status_last_command = execute_builtin(node, shell);
    else
    {
        pid = fork();
        if(pid == -1)
        {
            perror("minishell: pipe");
            shell->status_last_command = 1;
            return ;
        }
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            handle_redirections(node, in_fd, out_fd, shell);
            if (node->cmd == NULL)
                exit (0);
            if (check_if_builtin(node))
                execute_builtin_exit(node, shell);
            check_command_access(node);
            execve(node->cmd_path, node->args, shell->export);
            perror("minishell: execve");
            exit(1);
        }
        shell->pid[shell->pid_index++] = pid;
    }
}

static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    int pipe_fd[2];

    if (node->type == NODE_PIPE)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            return ;
        }
        execute_ast(shell, node->left, in_fd, pipe_fd[1]);
        close(pipe_fd[1]);
        execute_ast(shell, node->right, pipe_fd[0], out_fd);
        close(pipe_fd[0]);
    }
    else if(node->type == NODE_COMMAND)
        execute_command(shell, node, in_fd, out_fd);
}

void execute_pipeline(t_shell *shell)
{
    int in_fd;
    int out_fd;
    int status;
    int i;

    i = 0;
    shell->pid = malloc(sizeof(pid_t) * (shell->pipe_count + 1));
    if (!shell->pid)
    {
	    perror("minishell: malloc failed");
        return ;
    }
    shell->pid_index = 0;
    in_fd = STDIN_FILENO;
    out_fd = STDOUT_FILENO;
    signal(SIGINT, SIG_IGN);
    scan_heredocs(shell->node);
    prescan_redirections(shell->node);
    execute_ast(shell, shell->node, in_fd, out_fd);
    while(i < shell->pid_index)
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
    setup_signal_handlers();
}
