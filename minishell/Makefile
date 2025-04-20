# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 02:29:51 by omaezzem          #+#    #+#              #
#    Updated: 2025/04/20 15:23:28 by omaezzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

MAIN_SOURCES = execution/builtins/builtin_echo.c\
			   execution/builtins/builtin_pwd.c\
			   execution/utilis_exe/ft_strcmp.c\
			   execution/utilis_exe/free_split.c\
			   execution/utilis_exe/ft_strdup.c\
			   execution/utilis_exe/ft_strlen.c\
			   execution/utilis_exe/ft_lstadd_back.c\
			   execution/utilis_exe/ft_lstnew.c\
			   execution/utilis_exe/ft_split.c\
			   execution/env/env.c\
			   minishell.c\

MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)

all : $(NAME)

$(MAIN_OBJECTS):%.o: %.c include/minishell.h
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME) : $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) $(MAIN_OBJECTS) -o $(NAME)

clean :
	@rm -f $(MAIN_OBJECTS)

fclean : clean
	@rm -f $(NAME)

re : fclean all
