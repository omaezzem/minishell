/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_exp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:06:10 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/07 20:07:06 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exp_clear(t_exp **lst)
{
	t_exp	*current;
	t_exp	*next;

	if (!lst || !*lst)
		return;
	
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->vr)
			free(current->vr);
		if (current->vl)
			free(current->vl);
		free(current);
		current = next;
	}
	*lst = NULL;
}
