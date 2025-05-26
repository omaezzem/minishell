/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:17:48 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/26 18:14:32 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <fcntl.h>

# define FAILURE	0
# define SUCCESS	1
# define PMAX	4096
# define PATH_ENV "/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/goinfre/homebrew/bin:\
					/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
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
	TOKEN_SPACE,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_CMD,
}	t_type;


typedef struct s_env
{
    char			*var;
	int				fd;
	char			*val;
	struct  s_env	*next;
}				t_env;


typedef struct s_cmd
{
    char			**cmd;
    char			**args;
	char			**files;
	t_type			type;
	t_env			*env;
	char 			*value_expand;
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
int		ft_strlen(char *str);
char	*ft_strdup(char *s);
int		ft_isalnum(int c);
char	*ft_strjoin(char *s1, char *s2);
void	print_exe(char *str);
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
int		ft_isalpha(int c);
void	*ft_memcpy(void *dst, const void *src, size_t n);
int		ft_len_redirections(char **redirections);
int     ft_output_append(char **file, char **redirections, int i);
int     ft_inp_heredoc(char **file, char **redirections, int i);
char	*ft_itoa(int n);

/*---------------------------------------------execution--------------------------------------------*/

int		ft_echo(char **args);
int		ft_cd(t_env *env, char **args);
t_env	*ft_create_env(char **env, t_env **ev);
int		builtin_env(t_env **env_head, char **args);
void	ft_exit(t_cmd *data, char **args);
char	*find_env(t_env *env, char *var);
int		execute_single_cmd(t_env *env, char **envp, t_cmd *data);
int		ft_pwd(t_env *env);
int		ft_export(t_exp *exp, t_env *env, char **args);
t_exp	**ft_create_env_export(char **env, t_exp **list);
void	ft_unset(t_exp **exp, t_env **env, char **args);
void	to_single_redirection(char **files, char **redirections);
int		ft_execute(t_exp *exp, t_env *env, t_cmd *data, char **envp);
void	add_usr_bin_env(t_env **env_head);
void	add_path(t_env **env_head);
void	add_shlvl(t_env **env_head);
void	add_pwd(t_env **env_head);
void	minishell_invalid(char *invalid_str);
int 	ft_do_redirections(char **files, char **redirections);
void	update_val_env(t_env *env, char *var, char *val);
void	update_val_exp(t_exp *exp, char *var, char *val);
void	update_new(t_env *ev, char *newpath);
char	*determine_path(t_env *env, char **args, int lenargs);
void	update_old(t_env *ev, char *oldpath);
int		find_plus(char *var);
int		len_alnum_var(char *var);
int		find_equal(char *str);
int		len_at_first_equal(char *str);
int		len_equal(char *str);
void	if_double_var(t_exp **exp);
void	add_to_export_list_v(t_exp **exp, char *avzero);
void	add_to_export_list_vl(t_exp **exp, char *avzero, char *avone);
void	update_join_exp(t_exp *exp, char *var, char *val);
void	update_join_env(t_env *env, char *var, char *val);
void	add_to_export_list_vl(t_exp **exp, char *avzero, char *avone);
void	add_to_export_list_v(t_exp **exp, char *avzero);
void	add_to_env_list(t_env *env, char *args, char *avzero, char *avone);
t_exp	**ft_create_env_export(char **env, t_exp **list);
void	sort_exp_list(t_exp **exp);
void	minishell_invalid(char *invalid_str);
char	*check_var(char *var);
char	*args_zero(char *args);
char	*args_one(char *args);
int		search_var_in_exp(t_exp *exp, char *var);
int		search_var_in_env(t_env *env, char *var);


/*---------------------------------------------parsing--------------------------------------*/

#define MAX_ENV 50

# define ENOBRACE 1
# define ENOROOM 2
# define GRN  "\033[0;32m"

extern int	cmdline_shift;
extern int	cmdline_argc;
extern char	**cmdline_argv;

void	append_token(t_token **head, t_token *new_token);
t_token	*create_token(char *value, t_type type);
void	print_type(t_type type);
t_type	get_token_type(char *str);
int error(t_token *tokens, t_env *env);
char	*ft_substr(char *s, int start, int len);
int		error_pipe(t_token *tokens);
void	sigint_handler(int sig);
t_token	*tokenize(char *input);
char	*read_input(char *prompt);
t_cmd	*parse(t_env *env);
t_cmd *joining(t_token *tokens);
t_cmd *joining2(t_token *tokens);
char	*expand(t_token *token, t_env *env);
int		expand_env(char **oldp, char **newp, int brace_flag, char *var, t_env *env);
int		expand_pid(char **newp, int space_left);
int		expand_argv(char **oldp, char **newp, int space_left);
int		expand_status(char **newp, int space_left);
char	*gitenv(const char* name);
int		expand_argc(char **newp, int space_left);
int		is_match(char *str, char *ptrn);
int		expand_wildcard(char **oldp, char **newp, int space_left);

#endif
