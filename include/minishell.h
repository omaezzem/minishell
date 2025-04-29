/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:17:48 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/29 11:09:26 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

# define FAILURE	0
# define SUCCESS	1
# define PMAX	4096

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
	t_env			env;
	char			*workdir;
	char			**redirection;
	int				ex_status;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_exp
{
    char			*vr;
    char			*vl;
    struct  s_exp	*next;
}				t_exp;

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
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_lstadd_back_exp(t_exp **lst, t_exp *new);
int		ft_strcmp(char *s1, char *s2);
char	**mysplit(char *s, char c);
void	free_split(char **arr);
size_t	ft_strlen(char *str);
char	*ft_strdup(char *s);
int     ft_isalnum(int c);
char	*ft_strjoin(char *s1, char *s2);
void    print_exe(char *str);
int		mini_atoi(char *str);
void	ft_freeptr(void *ptr);
int		ft_isspace(char *str);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strchr(char *s, int c);
int		len_arg(char **args);
char	**ft_split(char *s, char c);
char	*ft_strchr_add_one(char *s, int c);
int		ft_isnum(int c);
void	ft_putstr_fd(char *s, int fd);

/*---------------------------------------------builtin---------------------------------------------*/

void	ft_echo(char **args);
int		ft_cd(t_cmd *data, char **args);
t_env	*ft_create_env(char **env, t_env **ev);
int		builtin_env(t_env *ev, char **args);
void	ft_exit(t_cmd *data, char **args);
char	*find_env(t_env *env, char *var);
int     ft_execute(t_cmd *data, char **args, char **env);
void    ft_pwd();
int     ft_export(t_env *env, char **args, char **envp);

/*---------------------------------------------parsing--------------------------------------*/

#define MAX_ENV 50

#define ENOBRACE 1
#define ENOROOM 2
# define GRN  "\033[0;32m"

extern int    cmdline_shift;
extern int    cmdline_argc;
extern char **cmdline_argv;

void	append_token(t_token **head, t_token *new_token);
t_token	*create_token(char *value, t_type type);
void	print_type(t_type type);
t_type	get_token_type(char *str);
int		error(t_token *tokens);
char	*ft_substr(char *s, unsigned int start, size_t len);
int		error_pipe(t_token *tokens);
void	sigint_handler(int sig);
t_token	*tokenize(char *input);
char	*read_input(char *prompt);
int		parse(void);
void	joining(t_token *tokens);
int		expand(t_token *token);
int expand_env(char **oldp, char **newp, int brace_flag);
int		expand_pid(char **newp, int space_left);
int		expand_argv(char **oldp, char **newp, int space_left);
int		expand_status(char **newp, int space_left);
char	*gitenv(const char* name);
int		expand_argc(char **newp, int space_left);
int		is_match(char *str, char *ptrn);
int		expand_wildcard(char **oldp, char **newp, int space_left);

 
#endif
