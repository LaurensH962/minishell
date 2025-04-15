# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ablodorn <ablodorn@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/28 16:36:18 by lhaas             #+#    #+#              #
#    Updated: 2025/04/15 12:51:38 by ablodorn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell
CFLAGS := -Wall -Wextra -Werror -g 
LDFLAGS := -lreadline
LIBFT := libft

HEADERS := -I$(LIBFT) -I.
LIBS := $(LIBFT)/libft.a

SRCS := $(shell find ./src -iname "*.c")
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
