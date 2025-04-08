
#include "minishell.h"

// test 42

void	execute_ast(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    int pipe_fd[2];

    if (node->type == NODE_PIPE)
    {
        if (pipe(pipe_fd) == -1)
        {
            cleanup_shell(shell);
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

void    execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd)
{
    pid_t pid;
    int status;

    pid = fork();
    if(pid == -1)
    {
        cleanup_shell(shell);
        perror("minishell: pipe");
        exit(1);
    }
    if (pid == 0)
    {
        handle_redirections(node, in_fd, out_fd, shell);
        if (node->cmd == NULL)
        exit (0);
        if (check_if_builtin(node))
            execute_builtin(node, shell);
        check_command_access(node, shell);
        execve(node->cmd_path, node->args, shell->export);
        perror("minishell: execve");
		cleanup_shell(shell);
		exit(1);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        shell->status_last_command = WEXITSTATUS(status);
    else if (WIFSIGNALED(status) && check_if_builtin(node) == 0)
    {
        shell->status_last_command = 128 + WTERMSIG(status);
        printf("%s: terminated by signal %d\n", node->cmd, WTERMSIG(status));
    }
    /*else
        shell->status_last_command = 123456789;*/

}

void     handle_redirections(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
    int fd_read;
    int fd_write;
    int heredoc_pipe[2];
    t_redirect *redir;

    redir = node->redirections;
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
    while(redir)
    {
        if (redir->type == NODE_REDIRECT_IN)
            handle_inputfile(&fd_read, node, shell);
        if (redir->type == NODE_REDIRECT_OUT)
            handle_outputfile(&fd_write, node, shell);
        if (redir->type == NODE_APPEND)
            handle_outputfile(&fd_write, node, shell);
        if (redir->type == NODE_HEREDOC)
            handle_heredoc(heredoc_pipe, node, shell);
        redir = redir->next;
    }
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
