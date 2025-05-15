#include "minishell.h"

static t_ast    *init_command(t_ast *command_node, t_token **tokens)
{
    command_node->type = NODE_COMMAND;
    command_node->cmd_path = NULL;
    return (parse_redirection(tokens, command_node, NULL, NULL));
}

static int  parse_command_helper(t_token **tokens, t_token *token,
        t_ast *command_node)
{
    //if (token->type == TOKEN_INVALID)
    token = skip_invalid_node(token, tokens);
    while (*tokens && is_redirect((*tokens)->type))
        command_node = parse_redirection(tokens, command_node, NULL, NULL);
    if (*tokens == NULL || ((*tokens)->type != TOKEN_WORD
            && (*tokens)->type != TOKEN_INVALID))
        return (-1);
    return (0);
}

t_ast   *parse_command(t_token **tokens, t_token *token, int arg_count,
        int args_capacity)
{
    t_ast   *command_node;

    command_node = ft_calloc(1, sizeof(t_ast));
    command_node = init_command(command_node, tokens);
    if ((*tokens)->type == TOKEN_INVALID)
        token = skip_invalid_node(token, tokens);
    if (*tokens == NULL || ((*tokens)->type != TOKEN_WORD
            && (*tokens)->type != TOKEN_INVALID) || !command_node)
        return (command_node);
    token = get_next_token(tokens);
    command_node->cmd = ft_strdup(token->value);
    command_node->args = ft_calloc(sizeof(char *), args_capacity);
    command_node->args[arg_count++] = ft_strdup(token->value);
    while (*tokens)
    {
        if (parse_command_helper(tokens, token, command_node) == -1)
            break ;
        if (arg_count >= args_capacity - 1)
            command_node->args = resize_args(command_node->args,
                    &args_capacity);
        token = get_next_token(tokens);
        command_node->args[arg_count++] = ft_strdup(token->value);
    }
    command_node->args[arg_count] = NULL;
    return (command_node);
}

t_ast   *parse_pipeline(t_token **tokens, t_ast *left_command,
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

t_ast   *parse(t_token *tokens)
{
    t_ast   *tree;

    tree = parse_pipeline(&tokens, NULL, NULL, NULL);
    if (!tree)
        return (NULL);
    return (parse_redirection(&tokens, tree, NULL, NULL));
}
