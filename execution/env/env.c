/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:03:16 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:18:52 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*give_env(char **env, t_env **ev)
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
			continue ;
		}
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (free_split(eqsplit), NULL);
		new_node->var = ft_strdup(eqsplit[0]);
		new_node->val = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		(ft_lstadd_back_env(ev, new_node), free_split(eqsplit));
	}
	return (*ev);
}

t_env	*ft_create_env(char **env, t_env **ev)
{
	t_env	*new_list;

	*ev = NULL;
	if (env[0] == NULL)
		return (add_pwd(ev), add_path(ev), add_usr_bin_env(ev),
			add_shlvl(ev), *ev);
	else
	{
		new_list = give_env(env, ev);
	}
	return (new_list);
}

char	*find_env(t_env *env, char *var)
{
	t_env	*curr;

	curr = env;
	while (curr != NULL)
	{
		if (!ft_strcmp(curr->var, var))
		{
			return (curr->val);
		}
		curr = curr->next;
	}
	return (NULL);
}
