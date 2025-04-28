
#include "minishell.h"


void handle_sigint_heredoc(int sig)
{
    (void)sig;
    write(1,"^C",2);
    g_rl_interrupted = 2;
}

int heredoc_event_hook(void)
{
    if (g_rl_interrupted)
    {
        rl_done = 1;
    }
    return (0);
}

static int fill_heredoc(t_redirect *redir, char *name, t_shell *shell)
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
    rl_event_hook = heredoc_event_hook;
    fd = open(name, O_CREAT | O_RDWR | O_EXCL, 0640);
    if (fd == -1)
        return (fd);
    while (1)
    {
        line = readline("> ");
        if (!line || g_rl_interrupted)
            break;
        if (ft_strcmp(line, redir->file) == 0)
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
        free(line);
    }

    rl_event_hook = NULL;
    close(fd);
    sigaction(SIGINT, &old_sa, NULL);
    if (g_rl_interrupted == 2)
    {
        shell->status_last_command = 130;
        unlink(name);
        return (-1);
    }
    fd = open(name, O_RDWR, 0640);
    unlink(name);
    return (fd);
}

static void	scan_heredocs(t_ast *node, t_shell *shell)
{
	t_redirect	*redir;
	char		temp_name[25];
	char		*num_str;
	static int	i;
    
    i = 0;
	redir = node->redirections;
	while (redir)
	{
		if (redir->type == NODE_HEREDOC  &&  g_rl_interrupted == 0)
		{
			i++;
			ft_strlcpy(temp_name, "tempfile_", sizeof(temp_name));
			num_str = ft_itoa(i);
			if (num_str)
			{
				ft_strlcat(temp_name, num_str, sizeof(temp_name));
				free(num_str);
			}
			redir->fd_heredoc = fill_heredoc(redir, temp_name, shell);
		}
		redir = redir->next;
	}
	if (node->left)
        scan_heredocs(node->left, shell);
	if (node->right)
        scan_heredocs(node->right, shell);
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
            //else if (redir->type == NODE_REDIRECT_IN)
            else
                flags = O_CREAT | O_WRONLY | O_APPEND;
            fd = open(redir->file, flags, 0644);
            if (fd != -1)
                close(fd);
        }
        redir = redir->next;
    }
    if (node->left)
        prescan_redirections(node->left);
    if (node->right)
        prescan_redirections(node->right);
}

void redir_close(int in_fd, int out_fd)
{
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        //close(in_fd);
    }
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        //close(out_fd);
    }
}


/*static void   execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    pid_t pid;

    if(check_if_builtin(node) && shell->pipe_count == 0)
    {
        if(handle_redirections_builtin(node, in_fd, out_fd))
        {
            shell->status_last_command = 1;
            return ;
        }
        shell->status_last_command = execute_builtin(node, shell);
    }
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
            handle_redirections(node, in_fd, out_fd);
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
}*/

static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    if (node->type == NODE_PIPE)
    {
        if (pipe(shell->pipes[shell->pipe_index]) == -1)
        {
            perror("minishell: pipe");
            return ;
        }
        shell->pipe_index++;
        execute_ast(shell, node->left, in_fd, shell->pipes[shell->pipe_index - 1][1]);
        execute_ast(shell, node->right, shell->pipes[shell->pipe_index - 1][0], out_fd);
        close(shell->pipes[shell->pipe_index - 1][1]); 
        close(shell->pipes[shell->pipe_index - 1][0]);
    }
    else if(node->type == NODE_COMMAND)
        execute_command(shell, node, in_fd, out_fd);
    if (in_fd != STDIN_FILENO)
        close(in_fd);
    if (out_fd != STDOUT_FILENO)
        close(out_fd);
}

static void	wait_for_children(t_shell * shell)
{
	int i;
	int status;

	i = 0;
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
}

void execute_pipeline(t_shell *shell)
{
    int in_fd;
    int out_fd;

    shell->pid = malloc(sizeof(pid_t) * (shell->pipe_count + 1));
    if (!shell->pid)
    {
	    perror("minishell: malloc failed");
        shell->status_last_command = 1;
        return ;
    }
    shell->pid_index = 0;
    in_fd = STDIN_FILENO;
    out_fd = STDOUT_FILENO;
    signal(SIGINT, SIG_IGN); //not sure if correctly placed
    scan_heredocs(shell->node, shell);
    if (g_rl_interrupted != 0)
    {
        g_rl_interrupted = 0;
        return ;
    }
    if (shell->pipe_count > 0)
    {
        shell->pipes = malloc(sizeof (int *) * shell->pipe_count);
        if (!shell->pipes)
        {
            perror("minishell: malloc failed");
            shell->status_last_command = 1;
            return ;
        }
        shell->pipe_index = 0;
        int i = 0;
        while(i < shell->pipe_count)
        {
            shell->pipes[i] = malloc(sizeof(int) * 2);
            if (!shell->pipes[i])
            {
                perror("minishell: malloc failed");
                shell->status_last_command = 1;
                return ;
            }
            i++;
        }
    }
    prescan_redirections(shell->node);
    execute_ast(shell, shell->node, in_fd, out_fd);
    wait_for_children(shell);
    free(shell->pid);
    setup_signal_handlers();
}
