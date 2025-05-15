#include "minishell.h"

static t_token  *inner_loop(int *pos, char *quote_char, char **token_value,
        t_lexer *lexer)
{
    while (lexer->input[*pos])
    {
        if (inner_quotes_expand(quote_char, lexer, pos, token_value) == -1)
            continue ;
        else if (!*quote_char && (ft_isspace(lexer->input[*pos])
                || !is_delimiter(lexer->input[*pos])))
        {
            if (*token_value == NULL)
                return (new_token(TOKEN_INVALID, ""));
            break ;
        }
        if (*token_value != NULL)
            *token_value = ft_strncat(*token_value, &lexer->input[*pos], 1);
        else
        {
            *token_value = ft_strndup(&lexer->input[*pos], 1);
            if (!*token_value)
                return (new_token(TOKEN_ERROR, "Memory allocation error"));
        }
        (*pos)++;
    }
    if (*token_value == NULL)
        return (new_token(TOKEN_INVALID, ""));
    return (NULL);
}

t_token *lexer_next_token(t_lexer *lexer, t_token *temp_token, char quote_char,
        char *token_value)
{
    lexer->was_quoted = 0;
    lexer->was_expanded = 0;
    while (lexer->input[lexer->pos])
    {
        if (lexer_skip_whitespaces(lexer, quote_char))
            continue ;
        temp_token = lexer_try_delim_token(lexer, quote_char);
        if (temp_token)
            return (temp_token);
        token_value = ft_calloc(1, ft_strlen(lexer->input) + 1);
        if (!token_value)
            return (new_token(TOKEN_ERROR, "Memory allocation error"));
        temp_token = inner_loop((int *)&lexer->pos, &quote_char, &token_value,
                lexer);
        if (temp_token)
            return (free(token_value), temp_token);
        if (quote_char)
            return (free(token_value), new_token(TOKEN_ERROR,
                    "Unclosed quote"));
        if (token_value == NULL)
            continue ;
        temp_token = lexer_process_token_value(lexer, token_value);
        return (free(token_value), temp_token);
    }
    return (free(token_value), new_token(TOKEN_EOF, NULL));
}


static t_token  *lexer_process_tokens(t_lexer *lexer, t_token **tokens,
        t_shell *shell)
{
    t_token *current_token;

    current_token = lexer_next_token(lexer, NULL, '\0', NULL);
    if (current_token->type == TOKEN_PIPE)
        shell->pipe_count++;
    while (current_token->type != TOKEN_EOF)
    {
        if (current_token->type != TOKEN_HEREDOC && lexer->hereflag == '<')
            lexer->hereflag = '\0';
        if (current_token->type == TOKEN_ERROR)
        {
            add_token(tokens, current_token);
            return (*tokens);
        }
        add_token(tokens, current_token);
        current_token = lexer_next_token(lexer, NULL, '\0', NULL);
        if (current_token->type == TOKEN_PIPE)
            shell->pipe_count++;
        if (!current_token)
            return (NULL);
    }
    add_token(tokens, current_token);
    return (*tokens);
}

static void init_lexer(t_token **tokens, t_lexer *lexer, t_shell *shell,
    char *line)
{
    *tokens = NULL;
    lexer->input = line;
    lexer->pos = 0;
    lexer->shell = shell;
    lexer->hereflag = '\0';
}

t_token *lexer(char *line, t_shell *shell)
{
    t_lexer lexer;
    t_token *tokens;

    tokens = NULL;
    init_lexer(&tokens, &lexer, shell, line);
    return (lexer_process_tokens(&lexer, &tokens, shell));
}