/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:39:19 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:56:55 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	add_oldpwd(t_exp **exp)
{
	t_exp	*new_node;

	new_node = malloc(sizeof(t_exp));
	if (!new_node)
		return ;
	new_node->vr = ft_strdup("OLDPWD");
	new_node->vl = NULL;
	new_node->next = NULL;
	ft_lstadd_back_exp(exp, new_node);
}

void	add_shlvlexp(t_exp **exp)
{
	t_exp	*new_node;

	new_node = malloc(sizeof(t_exp));
	if (!new_node)
		return ;
	new_node->vr = ft_strdup("SHLVL");
	new_node->vl = ft_strdup("1");
	new_node->next = NULL;
	ft_lstadd_back_exp(exp, new_node);
}

void	add_pwdexp(t_exp **exp)
{
	t_exp	*new_node;
	char	cwd[PATH_MAX];

	new_node = malloc(sizeof(t_exp));
	if (!new_node)
		return ;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd");
	new_node->vr = ft_strdup("PWD");
	new_node->vl = ft_strdup(cwd);
	new_node->next = NULL;
	ft_lstadd_back_exp(exp, new_node);
}

t_exp	*give_exp(char **env, t_exp **list)
{
	int		i;
	char	**eqsplit;
	t_exp	*new_node;

	i = -1;
	while (env[++i])
	{
		eqsplit = mysplit(env[i], '=');
		if (!eqsplit || !eqsplit[0] || !eqsplit[1])
		{
			free_split(eqsplit);
			continue ;
		}
		new_node = malloc(sizeof(t_exp));
		if (!new_node)
			return (free_split(eqsplit), NULL);
		new_node->vr = ft_strdup(eqsplit[0]);
		new_node->vl = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		ft_lstadd_back_exp(list, new_node);
		free_split (eqsplit);
	}
	return (*list);
}

t_exp	*ft_create_env_export(char **env, t_exp **list)
{
	t_exp	*new_list;

	*list = NULL;
	if (env[0] == NULL)
		return (add_oldpwd(list), add_pwdexp(list), add_shlvlexp(list), *list);
	else
	{
		new_list = give_exp (env, list);
	}
	return (new_list);
}
