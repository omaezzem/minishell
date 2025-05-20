/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:36:47 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/19 12:46:36 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_output_append(char **files, char **redirections, int i)
{
    int fd;

    if (!redirections[0] || !files[0])
        return FAILURE;

    if (ft_strcmp(redirections[i], ">") == 0)
    {
        fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
    }
    else if (ft_strcmp(redirections[i], ">>") == 0)
    {
        fd = open(files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
    }
    return SUCCESS;
}

int ft_inp_heredoc(char **files, char **redirections, int i)
{
    int fd;

    if (!redirections[0] || !files[0])
        return FAILURE;

    if (ft_strcmp(redirections[i], "<") == 0)
    {
        fd = open(files[i], O_RDONLY);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
    }
    else if (ft_strcmp(redirections[i], "<<") == 0)
    {
        fd = open(files[i], O_RDONLY);
        if (fd == -1)
            return (perror("minishell"), FAILURE);
    }
    return SUCCESS;
}
