/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablodorn <ablodorn@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:58:55 by lhaas             #+#    #+#             */
/*   Updated: 2025/04/11 15:10:52 by ablodorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

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
	int					status_last_command;
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

t_ast					*parse(t_token *tokens);

void					free_structs(t_shell *shell);
void					print_tokens(t_token *tokens);

// utils
int						is_delimiter(char c);
bool					is_redirect(t_token_type type);
char					*ft_strjoin_minishell(char const *s1, char const *s2,
							t_lexer *lexer);

// commmand + pipes
void					execute_pipeline(t_shell *shell);
void					handle_heredoc(int *heredoc_pipe, t_ast *node,
							t_shell *shell);
void					handle_inputfile(int *fd_read, t_ast *node);
void					handle_outputfile(int *fd_write, t_ast *node);
void					execute_builtin(t_ast *node, t_shell *shell);
int						check_if_builtin(t_ast *node);
void					handle_redirections(t_ast *node, int in_fd, int out_fd,
							t_shell *shell);
void					execute_command(t_shell *shell, t_ast *node, int in_fd,
							int out_fd);
void					execute_ast(t_shell *shell, t_ast *node, int in_fd,
							int out_fd);
// char	*get_command_path(char *cmd, char **envp, int *fail_flag);
int						set_command_path(t_ast *node, t_shell *shell);
// void					handle_path(t_shell *shell, t_ast *node);

// access

void					check_file_access_read(char *filename);
void					check_file_access_write(char *filename);
void					check_command_access(t_ast *node);
int						command_is_path(char *argv);
int						current_path(char *command);

// env

int						ft_setenv(const char *key, const char *value,
							char ***envp);
int						copy_environ(char **envp, char ***env);
void					export_default_variables(t_shell *shell);

// builtins

int						ft_export(t_shell *shell, char **args);
int						ft_unset(t_shell *shell, char **vars);
int						ft_env(t_shell *shell);
int						is_number(const char *str);
int						ft_echo(char **args);
int						ft_cd(t_shell *shell, t_ast *node);
void					ft_exit(char **args);
int						ft_pwd(void);
int						execute_builtin_env(t_ast *node, t_shell *shell);
int 					check_if_env_builtin(t_ast *node);
int						perror_malloc_return(void);
int						perror_cd_return(void);
//oid					set_pwd(t_shell *shell);
//void					set_oldpwd(t_shell *shell);
//void					set_default_home(t_shell *shell);

// printing
void					print_tokens(t_token *tokens);
void					print_ast(t_ast *node, int level);
void					report_error(const char *filename, const char *err_msg);

// cleanup

void					cleanup_shell(t_shell *shell);
void					cleanup_ast(t_ast *node);
void					free_array(char **array, int len);
void					free_split(char **split);

#endif


