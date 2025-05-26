/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:39:19 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/26 10:39:36 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_exp   **ft_create_env_export(char **env, t_exp **list)
{
	int		i;
	char	**eqsplit;
	t_exp	*new_node;

	i = -1;
	if (!env)
		return (NULL);
	while (env[++i])
	{
		eqsplit = mysplit(env[i], '=');
		if (!eqsplit || !eqsplit[0] || !eqsplit[1])
		{
			free_split(eqsplit);
			continue;
		}
		new_node = malloc(sizeof(t_exp));
		if (!new_node)
			return (free_split(eqsplit), NULL);
		new_node->vr = ft_strdup(eqsplit[0]);
		new_node->vl = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		ft_lstadd_back_exp(list, new_node);
		free_split(eqsplit);
	}
	return (list);
}
