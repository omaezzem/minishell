/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:40:40 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:41:20 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	swap_nodes(t_exp *a, t_exp *b)
{
	char	*temp_vr;
	char	*temp_vl;

	temp_vr = a->vr;
	a->vr = b->vr;
	b->vr = temp_vr;
	temp_vl = a->vl;
	a->vl = b->vl;
	b->vl = temp_vl;
}

int	sort_exp_list(t_exp **exp)
{
	t_exp	*current;
	t_exp	*next_node;

	if (!exp || !*exp)
		return (0);
	current = *exp;
	while (current != NULL)
	{
		next_node = current->next;
		while (next_node != NULL)
		{
			if (ft_strcmp(current->vr, next_node->vr) > 0)
				swap_nodes(current, next_node);
			next_node = next_node->next;
		}
		current = current->next;
	}
	return (1);
}
