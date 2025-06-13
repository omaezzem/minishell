/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_double_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:27:19 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:54:54 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	remove_duplicate(t_exp **prev, t_exp **compare)
{
	t_exp	*tmp;

	tmp = *compare;
	*compare = (*compare)->next;
	free(tmp->vr);
	free(tmp->vl);
	free(tmp);
	(*prev)->next = *compare;
}

static void	check_duplicates(t_exp *current)
{
	t_exp	*compare;
	t_exp	*prev;

	prev = current;
	compare = current->next;
	while (compare && compare->vr)
	{
		if (ft_strcmp(current->vr, compare->vr) == 0)
			remove_duplicate(&prev, &compare);
		else
		{
			prev = compare;
			compare = compare->next;
		}
	}
}

int	if_double_var(t_exp **exp)
{
	t_exp	*current;

	if (!exp || !*exp)
		return (0);
	current = *exp;
	while (current && current->vr)
	{
		check_duplicates(current);
		current = current->next;
	}
	return (1);
}
