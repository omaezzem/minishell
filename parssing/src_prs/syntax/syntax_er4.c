/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_er4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:21:11 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 19:10:30 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*make_heredoc_filename(t_gc *gc)
{
	int		i;
	char	*num;
	char	*tmp;
	char	*path;

	i = 0;
	while (1)
	{
		num = gc_itoa(gc, i);
		if (!num)
			return (NULL);
		tmp = gc_strjoin(gc, "/tmp/.heredoc_tmp_", num);
		if (!tmp)
			return (NULL);
		if (access(tmp, F_OK) != 0)
		{
			path = tmp;
			break ;
		}
		i++;
	}
	return (path);
}

int	is_invalid_pipe_sequence(t_token *cur)
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

void	write_expanded_line(t_ctx *ctx, char *line)
{
	char	*expanded;

	expanded = expand_heredoc(ctx, tokenize(ctx->gc, line));
	if (!expanded)
		exit(1);
	write(ctx->fd, expanded, ft_strlen(expanded));
	write(ctx->fd, "\n", 1);
}
