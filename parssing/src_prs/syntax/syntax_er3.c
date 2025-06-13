/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_er3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:19:04 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 20:59:06 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	should_expand_line(char *line, char *tmp, int expand_flag)
{
	return (expand_flag || (ft_strchr(line, '$')
			&& !(ft_strchr(tmp, '"') || ft_strchr(tmp, '\''))));
}

int	is_end_of_heredoc(char *line, char *delim)
{
	return (!line || ft_strcmp(line, delim) == 0);
}

void	read_and_write_heredoc(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (is_end_of_heredoc(line, ctx->delimeter))
		{
			free(line);
			break ;
		}
		if (should_expand_line(line, ctx->tmp, ctx->expand_flag))
			write_expanded_line(ctx, line);
		else
		{
			write(ctx->fd, line, ft_strlen(line));
			write(ctx->fd, "\n", 1);
		}
		free(line);
	}
}

void	handle_heredoc_child(t_gc *gc, t_ctx *ctx)
{
	t_ctx	input;
	char	*tmp;

	signal(SIGINT, SIG_DFL);
	tmp = gc_strdup(gc, ctx->delim->value);
	input.gc = gc;
	input.env = ctx->env;
	input.fd = ctx->fd;
	input.tmp = tmp;
	input.exit_status = ctx->cmd->ex_status;
	input.expand_flag = (*tmp == '$' && *(tmp + 1) != '\0');
	remove_quotes2_token(gc, ctx->delim);
	input.delimeter = ctx->delim->value;
	if (input.expand_flag && *input.delimeter == '$'
		&& *(input.delimeter + 1) == '\0')
		input.delimeter++;
	if (input.fd < 0)
		exit(1);
	read_and_write_heredoc(&input);
	close(ctx->heredoc->fd);
	exit(0);
}

int	open_heredoc_file(t_gc *gc, t_heredoc *heredoc)
{
	int	fd;

	heredoc->filename = make_heredoc_filename(gc);
	if (!heredoc->filename)
		return (-1);
	fd = open(heredoc->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (fd);
}
