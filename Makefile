# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 02:29:51 by omaezzem          #+#    #+#              #
#    Updated: 2025/04/29 16:45:13 by omaezzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------Source Files------------------------------------#
SRC = 	execution/builtins/builtin_cd.c \
		execution/builtins/builtin_pwd.c \
		execution/builtins/builtin_echo.c \
		execution/builtins/builtin_env.c \
		execution/builtins/builtin_exit.c \
		execution/builtins/builtin_export.c \
		execution/env/env.c \
		execution/utils_exe.c/free_ptr.c \
		execution/utils_exe.c/free_split.c \
		execution/utils_exe.c/ft_lstadd_back.c \
		execution/utils_exe.c/ft_lstnew.c \
		execution/utils_exe.c/mysplit.c \
		execution/src/ft_execute.c \
		parssing/utils_prs/append_token.c \
		parssing/utils_prs/create_token.c \
		parssing/src_prs/expand.c \
		parssing/src_prs/joining.c \
		parssing/src_prs/syntax_error.c \
		parssing/src_prs/tokenization.c \
		utils/atoi_mini.c \
		utils/ft_isspace.c \
		utils/ft_split.c \
		utils/ft_strchr.c \
		utils/ft_strcmp.c \
		utils/ft_strjoin.c \
		utils/ft_strdup.c \
		utils/ft_strlen.c \
		utils/ft_strncmp.c \
		utils/ft_substr.c \
		utils/ft_putstr_fd.c\
		utils/ft_isalnum.c\
		utils/ft_isnum.c\
		utils/ft_strchr_add_one.c\
		minishell.c\


#------------------------------Object Files------------------------------------#
OBJ = $(SRC:.c=.o)

#------------------------------Compiler and Flags------------------------------#
CC = cc
CFLAGS = -Wall -Wextra -Werror

#------------------------------Includes and Executables------------------------#
NAME = minishell

READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

#------------------------------Rules-------------------------------------------#
all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(READLINE_LINK)

%.o: %.c include/minishell.h
	@$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@
	# @echo █▀▄▀█ ░▀░ █▀▀▄ ░▀░ █░░█ █▀▀ █░░ █░░
	# @echo █░▀░█ ▀█▀ █░░█ ▀█▀ █▀▀█ █▀▀ █░░ █░░
	# @echo ▀░░░▀ ▀▀▀ ▀░░▀ ▀▀▀ ▀░░▀ ▀▀▀ ▀▀▀ ▀▀▀

#------------Clean-------------------------------------------#
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

#------------------------------Rebuild-----------------------------------------#
re: fclean all