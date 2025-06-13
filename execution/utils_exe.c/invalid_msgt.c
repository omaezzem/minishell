/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_msgt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:14:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:19:41 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
