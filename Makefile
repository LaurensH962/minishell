#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/28 16:36:18 by lhaas             #+#    #+#              #
#    Updated: 2025/04/30 17:38:21 by lhaas            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME := minishell
CFLAGS := -Wall -Wextra -Werror -g
LDFLAGS := -lreadline
LIBFT := libft

HEADERS := -I$(LIBFT) -I.
LIBS := $(LIBFT)/libft.a

SRCS := ./src/builtins/builtins_env.c ./src/builtins/builtins_utils.c ./src/builtins/env.c \
        ./src/builtins/ft_cd_utils.c ./src/builtins/ft_cd.c ./src/builtins/ft_echo_ft_pwd.c \
        ./src/builtins/ft_exit.c ./src/builtins/ft_export.c ./src/builtins/ft_setenv.c \
        ./src/command/access.c ./src/command/builtin_execution.c ./src/command/child.c \
        ./src/command/cleanup.c ./src/command/command_utils.c ./src/command/command.c \
        ./src/command/error.c ./src/command/execute_command.c ./src/command/free.c \
        ./src/command/redirections.c \
        ./src/heredoc/heredoc_add.c ./src/heredoc/heredoc_expand.c \
        ./src/heredoc/heredoc_handler.c ./src/heredoc/heredoc_signals.c \
        ./src/lexing/lexer.c ./src/lexing/expansion_utils.c ./src/lexing/expansion.c ./src/lexing/token.c \
		./src/lexing/lexer_utils.c \
        ./src/parsing/parser_tokens_redirs.c ./src/parsing/parser.c ./src/parsing/syntax_checker.c \
        ./src/parsing/utils.c ./src/parsing/parser_fill.c\
        ./src/path/path_utils.c ./src/path/path.c \
        ./src/utils/main_utils.c ./src/utils/perror.c \
        ./src/input_handling.c ./src/main.c
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)

$(NAME): $(OBJS) $(LIBFT)/libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)  # <-- No `-lreadline` here

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
