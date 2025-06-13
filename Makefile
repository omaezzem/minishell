# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/13 18:57:52 by omaezzem          #+#    #+#              #
#    Updated: 2025/06/13 18:57:55 by omaezzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC = 	execution/builtins/cd/builtin_cd.c \
		execution/builtins/cd/utils_cd.c\
		execution/builtins/builtin_pwd.c \
		execution/builtins/builtin_echo.c \
		execution/builtins/builtin_env.c \
		execution/builtins/builtin_exit.c \
		execution/builtins/builtin_unset.c \
		execution/builtins/ft_export/builtin_export.c\
		execution/builtins/ft_export/add_to.c\
		execution/builtins/ft_export/create_exp.c\
		execution/builtins/ft_export/parse_exp.c\
		execution/builtins/ft_export/rm_double_var.c\
		execution/builtins/ft_export/search.c\
		execution/builtins/ft_export/update.c\
		execution/builtins/ft_export/sort_exp.c\
		execution/builtins/ft_export/utils_export.c\
		execution/builtins/ft_export/parset_exp.c\
		execution/utils_exe.c/invalid_msgt.c\
		execution/builtins/ft_export/exptwo.c\
		execution/env/env.c \
		execution/utils_exe.c/free_ptr.c \
		execution/utils_exe.c/len_redirections.c\
		execution/src/infile_outfile.c\
		execution/src/is_dir_file.c\
		execution/src/is_bultin.c\
		execution/src/ft_excpone.c\
		execution/src/ft_excptwo.c\
		execution/utils_exe.c/msg_invalid.c\
		execution/utils_exe.c/free_split.c \
		execution/utils_exe.c/ft_lstadd_back.c \
		execution/utils_exe.c/ft_lstnew.c \
		execution/utils_exe.c/mysplit.c \
		execution/utils_exe.c/len_args.c\
		execution/src/ft_execute.c \
		execution/src/execute_single_cmd.c\
		execution/utils_exe.c/len_cmd.c\
		execution/utils_exe.c/ft_atoi.c\
		execution/src/single_redirect.c\
		execution/src/redirections.c\
		execution/utils_exe.c/ft_lstclear_env.c\
		execution/utils_exe.c/ft_lstclear_exp.c\
		parssing/utils_prs/append_token.c \
		parssing/utils_prs/create_token.c \
		parssing/utils_prs/quotes.c \
		parssing/utils_prs/spice.c \
		parssing/src_prs/joining/joining.c \
		parssing/src_prs/tokens/tokenization2.c \
		parssing/src_prs/joining/joining2.c \
		parssing/src_prs/syntax/syntax_error.c \
		parssing/src_prs/expand/expand.c \
		parssing/src_prs/tokens/tokens3.c \
		parssing/src_prs/tokens/tokenization.c \
		parssing/src_prs/syntax/syntax_er.c \
		parssing/src_prs/syntax/syntax_er2.c \
		parssing/src_prs/syntax/syntax_er3.c \
		parssing/src_prs/syntax/syntax_er4.c \
		parssing/src_prs/expand/expand_heredoc.c \
		parssing/src_prs/expand/expand2.c \
		parssing/src_prs/expand/expand3.c \
		utils/atoi_mini.c \
		utils/ft_bezero.c \
		utils/ft_isspace.c \
		utils/ft_split.c \
		utils/ft_strchr.c \
		utils/ft_strcmp.c \
		utils/ft_itoa.c \
		utils/ft_strjoin.c \
		utils/ft_strdup.c \
		utils/ft_strlen.c \
		utils/ft_memcpy.c \
		utils/ft_strncmp.c \
		utils/ft_substr.c \
		utils/ft_putstr_fd.c\
		utils/ft_isalnum.c\
		utils/ft_isnum.c\
		utils/ft_isalpha.c\
		utils/ft_strchr_add_one.c\
		parssing/garbage_collectore/garbage_collectore.c \
		minishell.c\

#------------------------------Object Files------------------------------------#
OBJ = $(SRC:.c=.o)

#------------------------------Compiler and Flags------------------------------#
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

#------------------------------Includes and Executables------------------------#
NAME = minishell

READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

#------------------------------Rules-------------------------------------------#
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(READLINE_LINK)  -o $(NAME)

%.o: %.c include/minishell.h
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@

#------------Clean-------------------------------------------#
clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

#------------------------------Rebuild-----------------------------------------#
re: fclean all
