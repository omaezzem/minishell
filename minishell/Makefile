# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 02:29:51 by omaezzem          #+#    #+#              #
#    Updated: 2025/03/14 02:36:25 by omaezzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

MAIN_SOURCES = execution/echo.c\
			   execution/pwd.c\
			   execution/utilis_exe/ft_strcmp.c

MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)

all : $(NAME)

$(MAIN_OBJECTS):%.o: %.c execution/minishell.h
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME) : $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) $(MAIN_OBJECTS) -o $(NAME)

clean :
	@rm -f $(MAIN_OBJECTS)

fclean : clean
	@rm -f $(NAME)

re : fclean all
