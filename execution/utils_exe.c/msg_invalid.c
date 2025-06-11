/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_invalid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:31:01 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 23:22:18 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    exit_success(t_cmd *data)
{
        data->ex_status = 0;
        exit(EXIT_SUCCESS);
}

void    exit_failure(t_cmd *data)
{
        data->ex_status = 1;
        exit(EXIT_FAILURE);
}
void    exit_one_handred_twenty_seven(t_cmd *data)
{
        data->ex_status = 127;
        exit(127);
}
void    exit_one_handred_twenty_sex(t_cmd *data)
{
        data->ex_status = 126;
        exit(126);
}

void	invalid_msg(char **cmd, t_cmd *data)
{
	if (cmd)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	data->ex_status = 127;
	exit(127);
}
void	invalid_path(t_cmd *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(": No sush file or directory\n", 2);
    data->ex_status = 127;
	exit(127);
}
