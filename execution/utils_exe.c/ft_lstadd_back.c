/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:17:29 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/21 09:17:29 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*crnt;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		crnt = *lst;
		while (crnt != NULL && crnt->next != NULL)
		{
			crnt = crnt->next;
		}
		if (crnt != NULL)
			crnt->next = new;
	}
}

void	ft_lstadd_back_exp(t_exp **lst, t_exp *new)
{
	t_exp	*crnt;

	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		crnt = *lst;
		while (crnt->next != NULL)
		{
			crnt = crnt->next;
		}
		crnt->next = new;
	}
}
