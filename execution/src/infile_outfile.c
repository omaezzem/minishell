/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:07:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 23:12:14 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fail_fd(int fd)
{
	if (fd == -1)
		return (perror("minishell")); 
}
static int	process_redirection(char *file, char *redir, int heredoc_fd, int *in_fd, int *out_fd)
{
	int	fd;

	if (ft_strcmp(redir, "<") == 0)
	{
		fd = open(file, O_RDONLY);
		fail_fd(fd);
		*in_fd = fd;
	}
	else if (ft_strcmp(redir, "<<") == 0)
		*in_fd = heredoc_fd;
	else if (ft_strcmp(redir, ">") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		fail_fd(fd);
		*out_fd = fd;
	}
	else if (ft_strcmp(redir, ">>") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		fail_fd(fd);
		*out_fd = fd;
	}
	return (SUCCESS);
}

int	ft_do_redirections(char **files, char **redirections, int heredoc, t_cmd *data)
{
	int	i;
	int	in_fd = -1;
	int	out_fd = -1;

	if (!files || !redirections)
		return (SUCCESS);
	if (ft_len_redirections(redirections) == 1)
		return (to_single_redirection(files, redirections, heredoc, data), 1);
	i = -1;
	while (files[++i])
	{
		if (process_redirection(files[i], redirections[i], heredoc, &in_fd, &out_fd) == FAILURE)
			return (FAILURE);
	}
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
		return (close(out_fd), perror("minishell"), FAILURE);
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
		return (close(in_fd), close(out_fd), perror("minishell"), FAILURE);
	if (out_fd != -1)
		close(out_fd);
	if (in_fd != -1 && in_fd != heredoc)
		close(in_fd);
	return (SUCCESS);
}
