/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:23:16 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/22 11:46:35 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void append_token(t_token **head, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
	{
		t_token *tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}
