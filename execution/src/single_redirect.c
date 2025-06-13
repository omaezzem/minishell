/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:01:11 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/13 18:29:07 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_output_append_s(char **files, char **redirections, t_cmd *data)
{
	int	fd;

	if (!redirections || !files || !files[0])
		return (FAILURE);
	if (ft_strcmp(redirections[0], ">") == 0)
	{
		fd = open(files[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror("minishell"), exit_failure(data), FAILURE);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("minishell"), FAILURE);
		close(fd);
	}
	else if (ft_strcmp(redirections[0], ">>") == 0)
	{
		fd = open(files[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (perror("minishell"), exit_failure(data), FAILURE);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("minishell"), FAILURE);
		close(fd);
	}
	return (SUCCESS);
}

int	ft_inp_heredoc_s(char **files, char **redirections,
		int heredoc, t_cmd *data)
{
	int	fd1;

	if (!redirections || !redirections[0] || !files || !files[0])
		return (FAILURE);
	if (ft_strcmp(redirections[0], "<<") == 0)
	{
		if (heredoc == -1)
			return (perror("heredoc"), FAILURE);
		if (dup2(heredoc, STDIN_FILENO) == -1)
			return (close(heredoc), perror("minishell"), FAILURE);
		close(heredoc);
	}
	else if (ft_strcmp(redirections[0], "<") == 0)
	{
		fd1 = open(files[0], O_RDONLY);
		if (fd1 == -1)
			return (perror("minishell"), data->flg = 1 , FAILURE);
		if (dup2(fd1, STDIN_FILENO) == -1)
			return (close(fd1), perror("minishell"), FAILURE);
		close(fd1);
	}
	return (SUCCESS);
}

void	to_single_redirection(char **files, char **redirections,
		int herdocfd, t_cmd *data)
{
	if (!files[0] || !redirections[0])
		return ;
	if ((ft_strcmp(redirections[0], ">") == 0)
		|| (ft_strcmp(redirections[0], ">>") == 0))
	{
		ft_output_append_s(files, redirections, data);
	}
	else if ((ft_strcmp(redirections[0], "<") == 0)
		|| (ft_strcmp(redirections[0], "<<") == 0))
	{
		ft_inp_heredoc_s(files, redirections, herdocfd, data);
	}
}
