/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:03:16 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/25 17:43:04 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env *ft_create_env(char **env, t_env **ev)
{
	int		i;
	char	**eqsplit;
	t_env	*new_node;
	
	i = -1;
	if (!env)
		return NULL;
	while (env[++i])
	{
		eqsplit = ft_split(env[i], '=');
		if (!eqsplit || !eqsplit[0] || !eqsplit[1])
		{
			free_split(eqsplit);
			continue;
		}
		new_node = malloc(sizeof(t_env));
		if (!new_node)
		{
			free_split(eqsplit);
			return NULL;
		}
		new_node->var = ft_strdup(eqsplit[0]);
		new_node->val = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		ft_lstadd_back_env(ev, new_node);
		free_split(eqsplit);
	}
	return *ev;
}
// for cd !!!!!!!!!!!!!!!
char	*find_env(t_env *env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (env->next != NULL)
	{
		if (ft_strncmp(tmp, env->var, ft_strlen(tmp)) == 0)
		{
			ft_freeptr(tmp);
			return (env->val + 1);
		}
		i++;
	}
	ft_freeptr(tmp);
	return (NULL);
}


