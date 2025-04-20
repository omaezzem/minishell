/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:43:20 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/20 17:40:41 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(t_env *ev, char **args)
{
	int		i;
	t_env	*tmp;

	*tmp = *ev;
	if (args && args[1])
		return 0;
	while (tmp)
	{
		printf("%s = %s\n", tmp->var, tmp->val);
		tmp = tmp->next;
	}
	return 1;
}
