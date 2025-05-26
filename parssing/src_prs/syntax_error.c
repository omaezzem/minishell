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

static int	write_heredoc_line(char *line, char *delimiter,
	int quoted, t_env *env, int fd)
{
	char	*to_write;

	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	to_write = line;
	if (!quoted && ft_strchr(line, '$'))
	{
		expand(tokenize(line), env);
		free(line);
	}
	write(fd, to_write, ft_strlen(to_write));
	write(fd, "\n", 1);
	if (to_write != line)
		free(to_write);
	return (0);
}

static int	handle_loop(int fd, char *delimiter, int quoted, t_env *env)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (-1);
		if (write_heredoc_line(line, delimiter, quoted, env, fd))
			break ;
	}
	return (0);
}
int	handle_heredoc(t_token *delimiter, t_env *env, int quoted)
{
	char	*tmp;
	int		fd;

	if (delimiter->value[0] == '\0')
	{
		fprintf(stderr,
			"minishell: warning: here-document delimited by end-of-file\n");
		return (-1);
	}
	remove_quotes2_token(delimiter);
	tmp = ft_strdup(delimiter->value);
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
    {
        perror("open");
        exit(1);
    }
	unlink(".heredoc_tmp");
	tokenize(tmp);
	if (handle_loop(fd, delimiter->value, quoted, env) < 0)
		return (free(tmp), close(fd), -1);
	return (free(tmp), fd);
}

int	is_redir_syntax_err(t_token *cur)
{
	int	t = cur->type;
	int	n = cur->next ? cur->next->type : -1;

	return ((t == TOKEN_REDIRECT_OUT && !cur->next)
		|| (t == TOKEN_REDIRECT_IN && !cur->next)
		|| (t == TOKEN_APPEND && !cur->next)
		|| (t == TOKEN_HEREDOC && !cur->next)
		|| (t == TOKEN_HEREDOC && n == TOKEN_SPACE && !cur->next->next)
		|| (t == TOKEN_APPEND && (n == TOKEN_REDIRECT_OUT || n == TOKEN_REDIRECT_IN))
		|| (t == TOKEN_REDIRECT_IN && n == TOKEN_APPEND)
		|| (t == TOKEN_HEREDOC && n == TOKEN_HEREDOC)
		|| (t == TOKEN_APPEND && n == TOKEN_APPEND)
		|| (t == TOKEN_REDIRECT_OUT && (n == TOKEN_REDIRECT_IN || n == TOKEN_HEREDOC))
		|| (t == TOKEN_HEREDOC && n == TOKEN_REDIRECT_OUT)
		|| (t == TOKEN_REDIRECT_OUT && n == TOKEN_REDIRECT_OUT)
		|| (t == TOKEN_REDIRECT_IN && n == TOKEN_REDIRECT_IN)
		|| (t == TOKEN_HEREDOC && n == TOKEN_SPACE && !cur->next->next)
		|| (t == TOKEN_APPEND && n == TOKEN_SPACE && !cur->next->next)
		// || (t == TOKEN_REDIRECT_IN && n == TOKEN_SPACE && cur->next->next->type == TOKEN_PIPE)
		// || (t == TOKEN_REDIRECT_OUT && n == TOKEN_SPACE && cur->next->next->type == TOKEN_PIPE)
		|| (t == TOKEN_APPEND && n == TOKEN_SPACE && cur->next->next->type == TOKEN_PIPE)
		|| (t == TOKEN_HEREDOC && n == TOKEN_SPACE && cur->next->next->type == TOKEN_PIPE)
		|| (t == TOKEN_REDIRECT_OUT && n == TOKEN_PIPE));
}
static int	handle_heredoc_token(t_token **cur, t_env *env)
{
	t_token	*delim;
	int		quoted;

	delim = (*cur)->next;
	while (delim && delim->type == TOKEN_SPACE)
		delim = delim->next;
	if (!delim || delim->type != TOKEN_FILE)
		return (printf("minishell: syntax error near unexpected token `newline'\n"), 0);
	if (delim->value[0] == '\0')
		return (*cur = delim->next, 1);
	quoted = is_quoted(delim->value);
	env->fd = handle_heredoc(delim, env, quoted);
	if (env->fd < 0)
		return (*cur = delim->next, 1);
	*cur = delim->next;
	return (1);
}

int	error(t_token *tokens, t_env *env)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc_token(&cur, env))
				return (0);
			continue ;
		}
		if (is_redir_syntax_err(cur))
			return (printf("minishell: syntax error near unexpected token '%s'\n",
					cur->value), 0);
		if (!error2(cur))
			return (0);
		cur = cur->next;
	}
	if (!error_pipe(tokens))
		return (0);
	return (1);
}
