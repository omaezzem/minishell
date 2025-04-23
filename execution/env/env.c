/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:03:16 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/23 10:10:16 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_create_env(char **env, t_env **ev)
{
	int		i;
	char	**eqsplit;
	t_env	*new_node;
	
	i = -1;
	while (env[++i])
	{
		eqsplit = ft_split(env[i], '=');
		if (!eqsplit || !eqsplit[0] || !eqsplit[1])
		{
			free_split(eqsplit);
			i++;
			continue;
		}
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->var = ft_strdup(eqsplit[0]);
		new_node->val = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		ft_lstadd_back(ev, new_node);
		free_split(eqsplit);
		i++;
	}
}
// for cd !!!!!!!!!!!!!!!
char	*find_env(char **env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	ft_freeptr(tmp);
	return (NULL);
}


