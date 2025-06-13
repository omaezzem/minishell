/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:46:02 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:18:34 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*ft_lstnew_val(char *val)
{
	t_env	*nodedyali;

	nodedyali = malloc(sizeof(t_env));
	if (!nodedyali)
		return (NULL);
	nodedyali->val = val;
	nodedyali->next = NULL;
	return (nodedyali);
	return (NULL);
}

t_env	*ft_lstnew_var(char *var)
{
	t_env	*nodedyali;

	nodedyali = malloc(sizeof(t_env));
	if (!nodedyali)
		return (NULL);
	nodedyali->val = var;
	nodedyali->next = NULL;
	return (nodedyali);
	return (NULL);
}
