/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_invalid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:31:01 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:20:44 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_success(t_cmd *data)
{
	data->ex_status = 0;
	exit(EXIT_SUCCESS);
}

void	exit_failure(t_cmd *data)
{
	data->ex_status = 1;
	exit(EXIT_FAILURE);
}

void	exit_one_handred_twenty_seven(t_cmd *data)
{
	data->ex_status = 127;
	exit(127);
}

void	exit_one_handred_twenty_sex(t_cmd *data)
{
	data->ex_status = 126;
	exit(126);
}
