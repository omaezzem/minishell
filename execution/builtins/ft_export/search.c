/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:30:09 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:47:26 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	search_var_in_env(t_env *env, char *var)
{
	t_env	*tmp;

	if (!env || !var)
		return (0);
	tmp = env;
	while (tmp != NULL)
	{
		if (tmp->var && ft_strcmp(tmp->var, var) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	search_var_in_exp(t_exp *exp, char *var)
{
	t_exp	*tmp;

	tmp = exp;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->vr, var) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
