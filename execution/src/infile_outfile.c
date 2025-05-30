/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:07:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/30 16:49:25 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	index_last_outfile(char **files, char **redirections)
{
	int	i;
	int	s;

	i = 0;
	s = 0; 
	while (files[i] != NULL)
	{
		if(redirections[i][0] == '>')
			s = i;
		i++;
	}
	return (s);
}
static int	index_last_infile(char **files, char **redirections)
{
	int	i;
	int	s;

	i = 0;
	s = 0; 
	while (files[i] != NULL)
	{
		if(redirections[i][0] == '<')
			s = i;
		i++;
	}
	return (s);
}

static int	process_outputs(char **files, char **redirections, int i_outfl)
{
	int	fd;
	int	i;

	i = -1;
	while (files[++i] && i < i_outfl)
	{
		if (ft_strcmp(redirections[i], ">") == 0
			|| ft_strcmp(redirections[i], ">>") == 0)
			ft_output_append(files, redirections, i);
	}
	if (files[i_outfl] && (ft_strcmp(redirections[i_outfl], ">") == 0
		|| ft_strcmp(redirections[i_outfl], ">>") == 0))
	{
		fd = ft_output_append(files, redirections, i_outfl);
		if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("minishell"), FAILURE);
	}
	return (SUCCESS);
}

static int	process_inputs(char **files, char **redirections, int i_infl, t_heredoc *heredoc)
{
	int	fd;
	int	i;

	i = -1;
	while (files[++i] && i < i_infl)
	{
		if (ft_strcmp(redirections[i], "<") == 0
			|| ft_strcmp(redirections[i], "<<") == 0)
			ft_inp_heredoc(files, redirections, i);
	}
	if (files[i_infl] && (ft_strcmp(redirections[i_infl], "<") == 0))
	{
		fd = ft_inp_heredoc(files, redirections, i_infl);
		if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
			return (close(fd), perror("minishell"), FAILURE);
	}
	else if (files[i_infl] && ft_strcmp(redirections[i_infl], "<<") == 0)
	{
		if (heredoc->fd == -1 || dup2(heredoc->fd, STDIN_FILENO) == -1)
			return (close(heredoc->fd), perror("minishell"), FAILURE);
	}
	return (SUCCESS);
}

int	ft_do_redirections(char **files, char **redirections, t_heredoc *heredoc)
{
	int	i_outfl;
	int	i_infl;

	if (!files || !redirections)
		return (SUCCESS);
	if (ft_len_redirections(redirections) == 1)
		return (to_single_redirection(files, redirections, heredoc), 1);
	i_outfl = index_last_outfile(files, redirections);
	i_infl = index_last_infile(files, redirections);
	if (process_outputs(files, redirections, i_outfl) == FAILURE)
		return (FAILURE);
	if (process_inputs(files, redirections, i_infl, heredoc) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
