/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:01:11 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/30 18:59:45 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_output_append_s(char **files, char **redirections)
{
    int fd;

    if (!redirections || !files || !files[0])
        return FAILURE;
        
    if (ft_strcmp(redirections[0], ">") == 0)
    {
        fd = open(files[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
        if (dup2(fd, STDOUT_FILENO) == -1)
            return (close(fd), perror("minishell"), FAILURE);
        close(fd);
    }
    else if (ft_strcmp(redirections[0], ">>") == 0)
    {
        fd = open(files[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
        if (dup2(fd, STDOUT_FILENO) == -1)
            return (close(fd), perror("minishell"), FAILURE);
        close(fd);
    }
    return SUCCESS;
}

int ft_inp_heredoc_s(char **files, char **redirections, t_heredoc *heredoc)
{
    int fd;

    if (!redirections[0] || !files[0])
        return FAILURE;
        
    if (ft_strcmp(redirections[0], "<") == 0)
    {
        fd = open(files[0], O_RDONLY);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
        if (dup2(fd, STDIN_FILENO) == -1)
            return (close(fd), perror("minishell"), FAILURE);
        close(fd);
    }
    else if (ft_strcmp(redirections[0], "<<") == 0)
    {
        if (heredoc->fd == -1)
            return (perror("minishell"), FAILURE);
        else if (dup2(heredoc->fd, 0) == -1)
            return (close(heredoc->fd), perror("minishell"), FAILURE);
        printf("----->%d\n", heredoc->fd);
        close(heredoc->fd);
    }
    return (SUCCESS);
}

void to_single_redirection(char **files, char **redirections, t_heredoc *heredoc)
{
    if (!files[0] || !redirections[0])
        return;
        
    if ((ft_strcmp(redirections[0], ">") == 0) || 
        (ft_strcmp(redirections[0], ">>") == 0))
    {
        ft_output_append_s(files, redirections);
    }
    else if ((ft_strcmp(redirections[0], "<") == 0) ||
             (ft_strcmp(redirections[0], "<<") == 0))
    {
        ft_inp_heredoc_s(files, redirections, heredoc);
    }
}  
