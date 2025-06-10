/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:02:18 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/07 20:05:54 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_env_clear(t_env **lst)
{
	t_env	*current;
	t_env	*next;

	if (!lst || !*lst)
		return;
	
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->var)
			free(current->var);
		if (current->val)
			free(current->val);
		free(current);
		current = next;
	}
	*lst = NULL;
}
