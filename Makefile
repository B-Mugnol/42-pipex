# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/06 19:11:32 by bmugnol-          #+#    #+#              #
#    Updated: 2022/04/01 04:45:03 by bmugnol-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	pipex

CC		:=	gcc
CFLAGS	:=	-Wall -Wextra -Werror

RM		:=	rm -rf

# PIPEX
SRC_DIR		:=	src
SRC_FILE	:=	pipex.c command.c error.c verifier.c dismount.c
SRC			:=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))

OBJ_DIR		:=	obj
OBJ_FILE	:=	$(SRC_FILE:.c=.o)
OBJ			:=	$(addprefix $(OBJ_DIR)/, $(OBJ_FILE))

HEADER_DIR	:=	inc
HEADER_FILE	:=	pipex.h define.h
HEADER		:=	$(addprefix $(HEADER_DIR)/, $(HEADER_FILE))
H_INCLUDE	:=	$(foreach directory, $(HEADER_DIR), -I$(directory))


# LIBFT
LIBFT_DIR		:=	../libft

LIBFT_H_DIR		:=	$(LIBFT_DIR)/inc
LIBFT_H_INC		:=	-I$(LIBFT_H_DIR)

LIBFT_LIB		:=	$(LIBFT_DIR)/libft.a


# Inclusions:
INCLUDE		:=	$(H_INCLUDE) $(LIBFT_H_INC)


.PHONY: all norm clean fclean re

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT_LIB) $(INCLUDE)

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

$(OBJ_DIR):
	@mkdir -p $@

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR) basic

norm:
	@$(MAKE) -C $(LIBFT_DIR) norm
	@norminette $(SRC) $(HEADER) | grep "Error" | cat

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all
