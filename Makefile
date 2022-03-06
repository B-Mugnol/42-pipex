# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/06 19:11:32 by bmugnol-          #+#    #+#              #
#    Updated: 2022/03/06 19:13:33 by bmugnol-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	pipex

CC		:=	gcc
CFLAGS	:=	-Wall -Wextra -Werror


# PIPEX
SRC_DIR		:=	./src
SRC_FILE	:=	pipex.c
SRC			:=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))

HEADER_FILE	:=	pipex.h
HEADER_DIR	:=	./inc
HEADER		:=	$(addprefix $(HEADER_DIR)/, $(HEADER_FILE))
H_INCLUDE	:=	$(foreach directory, $(HEADER_DIR), -I$(directory))

# All headers inclusion:
INCLUDE		:=	$(H_INCLUDE)


.PHONY: all norm clean fclean re

all: $(NAME)

$(NAME): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(INCLUDE)

norm:
	@$(MAKE) -C $(LIBFT_DIR) norm
	@norminette $(SRC) $(HEADER) | grep "Error" | cat

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(NAME)

re: fclean all
