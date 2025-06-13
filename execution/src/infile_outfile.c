/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:07:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:03:10 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fd_ou_in(int out_fd, int in_fd, int heredoc)
{
	if (out_fd != -1)
		close(out_fd);
	if (in_fd != -1 && in_fd != heredoc)
		close(in_fd);
}

static int	process_redirection_out(char *file, char *redir, int *out_fd)
{
	int	fd;

	if (ft_strcmp(redir, ">") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror("minishell"), 0);
		*out_fd = fd;
	}
	else if (ft_strcmp(redir, ">>") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (perror("minishell"), 0);
		*out_fd = fd;
	}
	return (SUCCESS);
}

static int	process_redirection_in(char *file, char *redir,
		int heredoc_fd, int *in_fd)
{
	int	fd;

	if (ft_strcmp(redir, "<") == 0)
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
			return (perror("minishell"), 0);
		*in_fd = fd;
	}
	else if (ft_strcmp(redir, "<<") == 0)
		*in_fd = heredoc_fd;
	return (SUCCESS);
}

int	ft_do_redirections(char **files, char **redirections,
		int heredoc, t_cmd *data)
{
	int	i;
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (!files || !redirections)
		return (SUCCESS);
	if (ft_len_redirections(redirections) == 1)
		return (to_single_redirection(files, redirections, heredoc, data), 1);
	i = -1;
	while (files[++i])
	{
		if (!process_redirection_out(files[i], redirections[i], &out_fd))
			return (FAILURE);
		if (!process_redirection_in(files[i], redirections[0], heredoc, &in_fd))
			return (FAILURE);
	}
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
		return (close(out_fd), perror("minishell"), FAILURE);
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
		return (close(in_fd), close(out_fd), perror("minishell"), FAILURE);
	close_fd_ou_in(out_fd, in_fd, heredoc);
	return (SUCCESS);
}
