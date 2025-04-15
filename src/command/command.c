
#include "minishell.h"

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

static void	set_exitstatus(t_shell *shell, t_ast *node, int status)
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
}

static void   execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    pid_t pid;
    int status;

    if(check_if_env_builtin(node))
        shell->status_last_command = execute_builtin_env(node, shell);
    else
    {
        pid = fork();
        signal(SIGINT, SIG_IGN);
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
                execute_builtin(node, shell);
            check_command_access(node);
            execve(node->cmd_path, node->args, shell->export);
            perror("minishell: execve");
            exit(1);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            set_exitstatus(shell, node, status);
        else if (WIFSIGNALED(status) /*&& check_if_builtin(node) == 0*/)
        {
            shell->status_last_command = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
                printf("\n");
            if (WTERMSIG(status) == SIGQUIT)
                printf("Quit (core dumped)\n");
        }
        setup_signal_handlers();
    }
}

static void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    int pipe_fd[2];

    if (node->type == NODE_PIPE)
    {
        if (pipe(pipe_fd) == -1)
        {
            //cleanup_shell(shell);
            perror("minishell: pipe");
            exit(1);
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

    in_fd = STDIN_FILENO;
    out_fd = STDOUT_FILENO;
    execute_ast(shell, shell->node, in_fd, out_fd);
    /*if (shell->status_last_command == 123456789)
        ft_putendl_fd("minishell: Error: failed to exit normally", 2);*/
}
