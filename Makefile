# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/06 19:11:32 by bmugnol-          #+#    #+#              #
#    Updated: 2022/03/29 06:13:35 by bmugnol-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	pipex
B_NAME	:=	pipex_bonus

CC		:=	gcc
CFLAGS	:=	-Wall -Wextra -Werror

RM		:=	rm -rf

# PIPEX
SRC_DIR		:=	./src
SRC_FILE	:=	pipex.c command.c error.c verifier.c dismount.c
SRC			:=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))

OBJ_DIR		:=	./obj
OBJ_FILE	:=	$(SRC_FILE:.c=.o)
OBJ			:=	$(addprefix $(OBJ_DIR)/, $(OBJ_FILE))

HEADER_FILE	:=	pipex.h define.h
HEADER_DIR	:=	./inc
HEADER		:=	$(addprefix $(HEADER_DIR)/, $(HEADER_FILE))
H_INCLUDE	:=	$(foreach directory, $(HEADER_DIR), -I$(directory))

# PIPEX BONUS
B_SRC_DIR	:=	$(addsuffix _bonus, $(SRC_DIR))
B_SRC_FILE	:=	$(SRC_FILE:.c=_bonus.c)
B_SRC		:=	$(addprefix $(B_SRC_DIR)/, $(B_SRC_FILE))

B_OBJ_DIR	:=	$(addsuffix _bonus, $(OBJ_DIR))
B_OBJ_FILE	:=	$(B_SRC_FILE:.c=.o)
B_OBJ		:=	$(addprefix $(B_OBJ_DIR)/, $(B_OBJ_FILE))

B_HEADER_FILE	:=	$(HEADER_FILE:.h=_bonus.h)
B_HEADER_DIR	:=	$(addsuffix _bonus, $(HEADER_DIR))
B_HEADER		:=	$(addprefix $(B_HEADER_DIR)/, $(B_HEADER_FILE))
B_H_INCLUDE		:=	$(foreach directory, $(B_HEADER_DIR), -I$(directory))


# LIBFT
LIBFT_DIR		:=	../libft

LIBFT_H_DIR		:=	$(LIBFT_DIR)/inc
LIBFT_H_INC		:=	-I$(LIBFT_H_DIR)

LIBFT_LIB		:=	$(LIBFT_DIR)/libft.a
LIBFT_LIB_INC	:=	-L $(LIBFT_DIR) -lft


# Inclusions:
INCLUDE		:=	$(H_INCLUDE) $(LIBFT_H_INC) $(LIBFT_LIB_INC)
B_INCLUDE	:=	$(B_H_INCLUDE) $(LIBFT_H_INC) $(LIBFT_LIB_INC)


.PHONY: all norm clean fclean re

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(INCLUDE)

bonus: $(B_NAME)

$(B_NAME): $(LIBFT_LIB) $(B_OBJ)
	$(CC) $(CFLAGS) -o $@ $(B_OBJ) $(B_INCLUDE)

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

$(B_OBJ): $(B_OBJ_DIR)/%.o: $(B_SRC_DIR)/%.c $(B_HEADER) | $(B_OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< $(B_INCLUDE)

$(OBJ_DIR) $(B_OBJ_DIR):
	@mkdir -p $@

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR) basic

norm:
	@$(MAKE) -C $(LIBFT_DIR) norm
	@norminette $(SRC) $(B_SRC) $(HEADER) $(B_HEADER) | grep "Error" | cat

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(B_OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME) $(B_NAME)

re: fclean all
