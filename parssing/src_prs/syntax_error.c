/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:19:36 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 22:15:33 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	error_pipe(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if ((cur->type == TOKEN_PIPE && !cur->next)
			|| cur->type == TOKEN_OR
			|| (cur->type == TOKEN_PIPE && cur->next->type == TOKEN_REDIRECT_OUT)
			|| (cur->type == TOKEN_PIPE && cur->next->type == TOKEN_SPACE
				&& cur->next->next
				&& cur->next->next->type == TOKEN_REDIRECT_OUT)
			|| (cur->type == TOKEN_PIPE && cur->next->type == TOKEN_PIPE)
			|| (cur->type == TOKEN_PIPE && cur->next->type == TOKEN_HEREDOC)
			|| (cur->type == TOKEN_PIPE && cur->next->type == TOKEN_REDIRECT_IN))
		{
			printf("minishell: syntax error near unexpected token '%s'\n",
				cur->value);
			return (0);
		}
		cur = cur->next;
	}
	return (1);
}

int	error2(t_token *cur)
{
	if (cur->type == TOKEN_REDIRECT_OUT && cur->next
		&& cur->next->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '%s'\n",
			cur->value);
		return (0);
	}
	if (cur->type == TOKEN_REDIRECT_IN && cur->next
		&& cur->next->type == TOKEN_REDIRECT_IN)
	{
		printf("minishell: syntax error near unexpected token '%s'\n",
			cur->value);
		return (0);
	}
	return (1);
}

static void	skip_quotes(const char *str, size_t *i, size_t *j, char *res)
{
	char	quote = str[(*i)++];

	while (str[*i] && str[*i] != quote)
		res[(*j)++] = str[(*i)++];
	if (str[*i] == quote)
		(*i)++;
}

char	*strip2_quotes(char *str)
{
	size_t	i = 0, j = 0;
	char	*res = malloc(ft_strlen(str) + 1);

	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			skip_quotes(str, &i, &j, res);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}



int	is_quoted(char *str)
{
	size_t	len = ft_strlen(str);

	return ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'));
}
char *generate_heredoc_filename(int index)
{
	char *base = ".heredoc_";
	char *num = ft_itoa(index); // تحويل index إلى string
	char *name = ft_strjoin(base, num);
	free(num);
	return (name);
}
void	remove_quotes2_token(t_token *token)
{
	char	*tmp;

	while (token)
	{
		if (token->value)
		{
			tmp = strip2_quotes(token->value);
			free(token->value);
			token->value = tmp;
		}
		token = token->next;
	}
}

// In write_heredoc_line:
// static int write_heredoc_line(char *line, char *delimiter,
//     int quoted, t_env *env, int fd)
// {
//     char *to_write;

//     if (ft_strcmp(line, delimiter) == 0) {
//         free(line);
//         return (1);
//     }
//     to_write = line;
//     if (!quoted && ft_strchr(line, '$')) {
//         to_write = expand_herdoc(tokenize(line), env);
//         free(line);
//     }
//     write(fd, to_write, ft_strlen(to_write));
//     write(fd, "\n", 1);
//     if (to_write != line) {
//         free(to_write);
//     }
//     return (0);
// }

static char *make_heredoc_filename(void)
{
    int     i = 0;
    char    *num;
    char    *tmp;
    char    *path;

    while (1) {
        num = ft_itoa(i);
        if (!num) return NULL;
        tmp = ft_strjoin("/tmp/.heredoc_tmp_", num);
        free(num);
        if (!tmp) return NULL;
        if (access(tmp, F_OK) != 0) {
            path = tmp;
            break;
        }
        free(tmp);
        i++;
    }
    return path;
}


static void handle_heredoc_child(t_token *delim, int fd)
{
    char *line;

    signal(SIGINT, SIG_DFL);
    if (fd < 0) exit(1);

    while (1) {
        line = readline("> ");
        if (!line || ft_strcmp(line, delim->value) == 0) {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    exit(0);
}

void handle_heredocs_range(t_token *start, t_heredoc *heredoc)
{
    t_token *curr = start;
    pid_t pid;
    int status;

    while (curr) {
        if (curr->type == TOKEN_HEREDOC) {
            t_token *delim = curr->next;
            if (!delim) break;
		
            remove_quotes2_token(delim);
            heredoc->filename = make_heredoc_filename();
            if (!heredoc->filename) return;
            int write_fd = open(heredoc->filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            if (write_fd < 0) {
                perror("minishell: heredoc");
                free(heredoc->filename);
                return;
            }

            pid = fork();
            if (pid == 0) {
                handle_heredoc_child(delim, write_fd);
            } else {
                waitpid(pid, &status, 0);
                close(write_fd);
                
                heredoc->fd = open(heredoc->filename, O_RDONLY);
                if (heredoc->fd < 0) {
                    perror("minishell: heredoc");
                    free(heredoc->filename);
                    return;
                }
                
                heredoc->flag_heredoc = 1;
                curr->type = TOKEN_REDIRECT_IN;
                free(delim->value);
                delim->value = heredoc->filename;
            }
        }
        curr = curr->next;
    }
}

int	is_redir_syntax_err(t_token *cur)
{
	int	t = cur->type;
	int	n = -1;

	if (cur->next)
		n = cur->next->type;

	return ((t == TOKEN_REDIRECT_OUT && !cur->next)
		|| (t == TOKEN_REDIRECT_IN && !cur->next)
		|| (t == TOKEN_APPEND && !cur->next)
		|| (t == TOKEN_HEREDOC && !cur->next)
		|| (t == TOKEN_APPEND && (n == TOKEN_REDIRECT_OUT || n == TOKEN_REDIRECT_IN))
		|| (t == TOKEN_APPEND && n == TOKEN_PIPE)
		|| (t == TOKEN_APPEND && n == TOKEN_HEREDOC)
		|| (t == TOKEN_HEREDOC && n == TOKEN_APPEND)
		|| (t == TOKEN_REDIRECT_IN && n == TOKEN_APPEND)
		|| (t == TOKEN_HEREDOC && n == TOKEN_HEREDOC)
		|| (t == TOKEN_APPEND && n == TOKEN_APPEND)
		|| (t == TOKEN_APPEND && n == TOKEN_HEREDOC)
		|| (t == TOKEN_REDIRECT_OUT && (n == TOKEN_REDIRECT_IN || n == TOKEN_HEREDOC))
		|| (t == TOKEN_HEREDOC && n == TOKEN_REDIRECT_OUT)
		|| (t == TOKEN_REDIRECT_IN && n == TOKEN_REDIRECT_IN)
		|| (t == TOKEN_REDIRECT_IN && cur->next && cur->next->type == TOKEN_PIPE)
		|| (t == TOKEN_REDIRECT_OUT && n == TOKEN_PIPE));
}


int	error(t_token *tokens, t_heredoc *heredoc)
{
	t_token	*cur = tokens;

	while (cur)
	{
		if (cur->type == TOKEN_HEREDOC && cur->next && cur->next->type != TOKEN_APPEND
		&& cur->next->type != TOKEN_HEREDOC)
		{
			heredoc->flag_heredoc = 1;
			handle_heredocs_range(cur, heredoc);
			continue;
		}
		if (is_redir_syntax_err(cur))
			return (printf("minishell: syntax error near unexpected token '%s'\n", cur->value), 0);
		if (!error2(cur))
			return (0);
		cur = cur->next;
	}
	if (!error_pipe(tokens))
		return (0);
	return (1);
}
