/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:57:31 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/28 19:57:56 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int len_cmd(t_cmd *data)
{
	int i;
	t_cmd *curr;

	curr = data;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return i;
}
