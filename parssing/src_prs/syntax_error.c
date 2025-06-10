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


void quote_handler(char *str, size_t *i, size_t *j, char *res)
{
    char quote = str[*i];
    (*i)++;
    while (str[*i] && str[*i] != quote)
    {
        res[(*j)++] = str[*i];
        (*i)++;
    }
    if (str[*i] == quote)
        (*i)++;
}
char *strip2_quotes(t_gc *gc, char *str)
{
	size_t	i = 0;
    size_t	j = 0;
	char	*res = gc_malloc(gc, ft_strlen(str) + 1); // بدل malloc بـ gc_malloc

	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			quote_handler(str, &i, &j, res);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}



void remove_quotes2_token(t_gc *gc, t_token *token)
{
	char *tmp;

	while (token)
	{
		if (token->value)
		{
			tmp = strip2_quotes(gc, token->value);
			// free(token->value); // بدل free بـ gc_free
			token->value = tmp;
		}
		token = token->next;
	}
}


static char *make_heredoc_filename(t_gc *gc)
{
    int i = 0;
    char *num;
    char *tmp;
    char *path;

    while (1) {
        num = gc_itoa(gc, i);
        if (!num) return NULL;
        tmp = gc_strjoin(gc, "/tmp/.heredoc_tmp_", num);
        // free(num);  // بدل free بـ gc_free
        if (!tmp) return NULL;
        if (access(tmp, F_OK) != 0) {
            path = tmp;
            break;
        }
        // free(tmp);  // بدل free بـ gc_free
        i++;
    }
    return path;
}


static int	is_invalid_pipe_sequence(t_token *cur)
{
	if (!cur->next)
		return (1);
	if (cur->next->type == TOKEN_PIPE || cur->next->type == TOKEN_OR)
		return (1);
	if (cur->next->type == TOKEN_SPACE
		&& cur->next->next
		&& (cur->next->next->type == TOKEN_PIPE
			|| cur->next->next->type == TOKEN_OR))
		return (1);
	return (0);
}

int	error_pipe(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE && is_invalid_pipe_sequence(cur))
		{
			printf("minishell: syntax error near unexpected token '%s'\n",
				cur->value);
			return (0);
		}
		if (cur->type == TOKEN_OR)
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
static void write_expanded_line(t_gc *gc, char *line, t_env *env, int fd)
{
	char *expanded = expand_herdoc(tokenize(gc, line), env);
	if (!expanded)
		exit(1);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	// free(expanded);
}

static int should_expand_line(char *line, char *tmp, int expand_flag)
{
	return (expand_flag || (ft_strchr(line, '$') && 
			!(ft_strchr(tmp, '"') || ft_strchr(tmp, '\''))));
}

static int is_end_of_heredoc(char *line, char *delim)
{
	return (!line || ft_strcmp(line, delim) == 0);
}

static void read_and_write_heredoc(t_gc *gc, int fd, char *delim, t_env *env, char *tmp, int expand_flag)
{
	char *line;

	while (1)
	{
		line = readline("> ");
		if (is_end_of_heredoc(line, delim))
		{
            printf("1\n");
            free(line);
			break;
		}
		if (should_expand_line(line, tmp, expand_flag))
        {
        write_expanded_line(gc, line, env, fd);
        }
		else
		{
            write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
		}
		free(line);
	}
}

static void handle_heredoc_child(t_gc *gc, t_token *delim, int fd, t_env *env)
{
	char *tmp = gc_strdup(gc, delim->value);
	int expand_flag = (*delim->value == '$' && *(delim->value + 1) != '\0');
	char *clean_delim;

	signal(SIGINT, SIG_DFL);
	remove_quotes2_token(gc, delim);
	clean_delim = delim->value;
	if (expand_flag && *clean_delim == '$' && *(clean_delim + 1) == '\0')
		clean_delim++;
	if (fd < 0)
		exit(1);
    
	read_and_write_heredoc(gc, fd, clean_delim, env, tmp, expand_flag);
	// free(tmp);
	close(fd);
	exit(0);
}
static int open_heredoc_file(t_gc *gc, t_heredoc *heredoc)
{
    int fd;

    heredoc->filename = make_heredoc_filename(gc);
    if (!heredoc->filename)
        return (-1);
    fd = open(heredoc->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("fd: %d\n", fd);
    if (fd < 0)
    {
        perror("minishell: heredoc");
        return (-1);
    }
    return (fd);
}

static int handle_heredoc_parent(pid_t pid, int write_fd,
        t_token *curr, t_token *delim, t_heredoc *heredoc)
{
    int status;

    waitpid(pid, &status, 0);
    close(write_fd);
    heredoc->fd = open(heredoc->filename, O_RDONLY);
    if (heredoc->fd < 0)
    {
        perror("minishell: heredoc");
        return (0);
    }
    heredoc->flag_heredoc = 1;
    curr->type = TOKEN_REDIRECT_IN;
    if (delim->value)
        delim->value = heredoc->filename;
    return (1);
}

static int	check_delim_validity(t_token *delim, t_heredoc *heredoc)
{
	if (!delim || delim->type != TOKEN_FILE)
	{
		heredoc->fd = -1;
		return (0);
	}
	return (1);
}

static int	fork_and_handle(t_gc *gc, t_token *curr, t_token *delim,
				t_heredoc *heredoc, t_env *env)
{
	pid_t	pid;
	int		write_fd;

	write_fd = open_heredoc_file(gc, heredoc);
	if (write_fd < 0)
		return (0);
	pid = fork();
	if (pid == 0)
		handle_heredoc_child(gc, delim, write_fd, env);
	else
	{
		if (!handle_heredoc_parent(pid, write_fd, curr, delim, heredoc))
			return (0);
	}
	return (1);
}

void	handle_heredocs_range(t_gc *gc, t_token *start,
			t_heredoc *heredoc, t_env *env)
{
	t_token	*curr;
	t_token	*delim;

	curr = start;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
		{
			delim = curr->next;
			if (!check_delim_validity(delim, heredoc))
				return ;
			if (!fork_and_handle(gc, curr, delim, heredoc, env))
				return ;
		}
		curr = curr->next;
	}
}


int is_redir_syntax_err(t_token *cur)
{
    int t = cur->type;
    int n = -1;

    if (!cur->next)
        return (t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
			|| t == TOKEN_APPEND || t == TOKEN_HEREDOC);

    n = cur->next->type;

    if (t == TOKEN_PIPE && cur->next->type == TOKEN_PIPE)
        return (1);
    if (t == TOKEN_HEREDOC && !cur->next)
        return (1);
    if ((t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
        || t == TOKEN_APPEND || t == TOKEN_HEREDOC)
        && (n == TOKEN_REDIRECT_IN || n == TOKEN_REDIRECT_OUT
        || n == TOKEN_APPEND || n == TOKEN_HEREDOC || n == TOKEN_PIPE))
        return (1);

    return (0);
}
int check_pipe_at_start(t_token *tokens)
{
	if (tokens && tokens->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '%s'\n", tokens->value);
		return (0);
	}
	return (1);
}

static int	handle_heredoc_logic(t_gc *gc, t_token *cur, t_heredoc *heredoc, t_env *env)
{
	if (cur->type == TOKEN_HEREDOC && cur->next
		&& cur->next->type != TOKEN_APPEND
		&& cur->next->type != TOKEN_HEREDOC)
	{
		heredoc->flag_heredoc = 1;
		handle_heredocs_range(gc, cur, heredoc, env);
		return (1);
	}
	return (0);
}


int	error(t_gc *gc, t_token *tokens, t_heredoc *heredoc, t_env *env)
{
	t_token	*cur;

	cur = tokens;
	if (!check_pipe_at_start(tokens))
		return (0);
    while (cur)
    {
        if (!error_pipe(tokens))
            return (0);
		if (handle_heredoc_logic(gc, cur, heredoc, env))
		{
			cur = cur->next;
			continue;
		}
		// if (cur->type == TOKEN_PIPE)
		// 	heredoc->fd = -1;
		if (is_redir_syntax_err(cur))
		{
			printf("minishell: syntax error near unexpected token '%s'\n",
				cur->value);
			return (0);
		}
		if (!error2(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}
