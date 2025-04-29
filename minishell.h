#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
#include <sys/stat.h>

//MAIN
int				main(int argc, char **argv, char **envp);

//Sigals
extern volatile sig_atomic_t g_rl_interrupted;

// lexing
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_EXIT_STATUS,
	TOKEN_ERROR,
	TOKEN_INVALID,
	TOKEN_EOF
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

// parsing
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_IN,
	NODE_APPEND,
	NODE_HEREDOC
}						t_node_type;

typedef struct s_redirect
{
	t_node_type			type;
	int					fd_heredoc;
	char				*file;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_ast
{
	t_node_type			type;
	char				*cmd;
	char				*cmd_path;
	char				**args;
	t_redirect			*redirections;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct s_shell
{
	char				**env;
	char				**export;
	t_ast				*node;
	int					pipe_count;
	pid_t				*pid;
	int					pid_index;
	int					**pipes;
	int					pipe_index;
	int					status_last_command;
	int					index;
	t_token				*tokens;

}						t_shell;

typedef struct s_lexer
{
	char				*input;
	char				hereflag;
	size_t				pos;
	t_shell				*shell;
	int					was_quoted;
	int					was_expanded;
}						t_lexer;

// expansion

void					lexer_expander(t_lexer *lexer, char **token_value);
t_token					*split_expanded_value(t_token *head, char *token_value,
							char *word);
char					*expand_variable(const char *input, size_t *pos,
							t_lexer *lexer);
char					*get_env_value(char **env, const char *var_name);

// expansion_utils
char					*handle_special_cases(const char *input, size_t *pos,
							t_lexer *lexer);
bool					extract_var_name(const char *input, size_t *start,
							char *var_name);
char					*lookup_and_expand(char *var_name, t_lexer *lexer);

// create tokens
t_token					*create_delim_token(t_lexer *lexer);
t_token					*new_token(t_token_type type, char *value);
void					add_token(t_token **head, t_token *new);
t_token					*lexer_try_delim_token(t_lexer *lexer, char quote_char);
t_token					*lexer_process_token_value(t_lexer *lexer,
							char *token_value);

// lexer
t_token					*lexer_next_token(t_lexer *lexer, t_token *temp_token,
							char quote_char, char *token_value);
t_token					*lexer(char *line, t_shell *shell);

// lexer_utils
bool					lexer_skip_whitespaces(t_lexer *lexer, char quote_char);
void					lexer_quotes(char *quote_char, t_lexer *lexer, int *pos,
							char **token_value);
char					*get_env_value(char **env, const char *var_name);

// parser
t_ast					*parse(t_token *tokens);
t_ast					*parse_redirection(t_token **tokens, t_ast *command,
							t_token *redirection_token,
							t_token *filename_token);
void					fill_redir_type(t_redirect *redir,
							t_token *redirection_token);
t_token					*get_next_token(t_token **tokens);
char					*syntax_checker(t_token *tokens);

void					free_structs(t_shell *shell);
void					print_tokens(t_token *tokens);

// utils
int						is_delimiter(char c);
bool					is_redirect(t_token_type type);
char					*ft_strjoin_minishell(char const *s1, char *s2,
							t_lexer *lexer);

// heredoc
void					handle_heredoc(int *fd_read);
int						handle_heredoc_builtin(int *fd_read);
void    				unlink_heredoc_fd(t_ast *node);

// commmand + pipes
void					execute_pipeline(t_shell *shell);
void					handle_inputfile(int *fd_read,
							t_redirect *redirections);
void					handle_outputfile(int *fd_write,
							t_redirect *redirections);
int						execute_builtin(t_ast *node, t_shell *shell);
void					execute_builtin_exit(t_ast *node, t_shell *shell);
int						check_if_builtin(t_ast *node);
int						set_command_path(t_ast *node, t_shell *shell);
void   					execute_command(t_shell *shell, t_ast *node, int in_fd, int out_fd);
char					*get_command_path(char *cmd, char **envp, int *fail_flag);

// access

int						check_file_access_write(char *filename, int i);
int						check_file_access_read(char *filename, int i);
void					check_command_access(t_ast *node);
int						command_is_path(char *argv);
int						current_path(char *command);
int 					is_directory(char *filename);

// env

int						ft_setenv(const char *key, const char *value,
							char ***envp);
int						copy_environ(char **envp, char ***env);

// builtins

int						ft_export(t_shell *shell, char **args);
int						ft_unset(t_shell *shell, char **vars);
int						ft_env(t_shell *shell, t_ast *node);
int						ft_echo(char **args);
int						ft_cd(t_shell *shell, t_ast *node);
void					ft_exit(char **args);
int						ft_pwd(void);
char	*get_oldpwd(t_shell *shell);

// builtin helpers

int						cd_minus(t_shell *shell);
int						is_number(const char *str);
int						is_valid_identifier(const char *str);
int						perror_malloc_return(void);
int						perror_cd_return(void);
int 					perror_free_return(char *function_name, char *string);
int						perror_malloc_free_return(char *key, char *value);

// signals
void					setup_signal_handlers(void);

//redirections

int						handle_redirections_builtin(t_ast *node, int in_fd,
	int out_fd);
int						handle_outputfile_builtin(int *fd_write,
	t_redirect *redirections);
int						handle_inputfile_builtin(int *fd_read,
	t_redirect *redirections);
void					redir_close(int in_fd, int out_fd);


// printing
void					print_tokens(t_token *tokens);
void					print_ast(t_ast *node, int level);
void					report_error(const char *filename, const char *err_msg);

// cleanup

void					cleanup_shell(t_shell *shell);
void					cleanup_ast(t_ast **node);
void					free_array(char **array, int len);
void					free_split(char **split);
void					free_tokens(t_shell *shell);
void					free_pipes(int **pipes, int count);

#endif
