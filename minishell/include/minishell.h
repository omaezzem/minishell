/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:17:48 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/20 19:52:18 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>


#include <readline/readline.h>
#include <readline/history.h>

#define GRN   "\033[0;32m"
#define RED   "\033[0;31m"
#define BLU   "\033[0;34m"

# define FAILURE 0
# define SUCCESS 1

typedef  enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_FILE,
	TOKEN_IN_FILE,
	TOKEN_OUT_FILE,
	TOKEN_OR,
	TOKEN_APPEND_FILE,
	TOKEN_HEREDOC_FILE,
	TOKEN_OPTION,
}	t_type;

typedef struct s_env
{
	char			*var;
	char			*val;
	struct  s_env	*next;
}				t_env;


typedef struct s_cmd
{
	char			**cmd;
	char			**option;
	t_type			type;
	char			**redirection;
	int				ex_status;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_token
{
	char *value;
	t_cmd **cmd;
	t_type type;
	struct s_token *next;
} t_token;

/*---------------------------------------------utils---------------------------------------------*/

t_env	*ft_lstnew_val(char *val);
t_env	*ft_lstnew_var(char *var);
void	ft_lstadd_back(t_env **lst, t_env *new);
int		ft_strcmp(char *s1, char *s2);
char	**ft_split(char *s, char c);
void	free_split(char **arr);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *s);

/*---------------------------------------------builtin---------------------------------------------*/
void	ft_echo(char **args);
void	ft_create_env(char **env, t_env **ev);
int		builtin_env(t_env *ev, char **args);

#endif