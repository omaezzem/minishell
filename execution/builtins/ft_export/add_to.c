/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:36:29 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:57:38 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	add_to_export_list_vl(t_exp **exp, char *avzero, char *avone)
{
	t_exp	*add;

	add = NULL;
	if (search_var_in_exp(*exp, avzero))
		update_val_exp(*exp, avzero, avone);
	add = malloc(sizeof(t_exp));
	if (!add)
		return ;
	add->vr = ft_strdup(avzero);
	if (avone)
		add->vl = ft_strdup(avone);
	else if (!avone)
		add->vl = ft_strdup("");
	add->next = NULL;
	ft_lstadd_back_exp(exp, add);
}

void	add_to_export_list_v(t_exp **exp, char *avzero)
{
	t_exp	*add;

	if (!exp || !avzero)
		return ;
	if (search_var_in_exp(*exp, avzero))
		return ;
	add = malloc(sizeof(t_exp));
	if (!add)
		return ;
	add->vr = ft_strdup(avzero);
	add->vl = NULL;
	add->next = NULL;
	ft_lstadd_back_exp(exp, add);
}

void	add_to_env_list(t_env *env, char *args, char *avzero, char *avone)
{
	t_env	*nnode;

	if (!env || !args || !avzero || !avone)
		return ;
	if ((find_equal(args) == 1))
	{
		if (search_var_in_env(env, avzero))
		{
			update_val_env(env, avzero, avone);
			return ;
		}
		nnode = malloc(sizeof(t_env));
		if (!nnode)
			return ;
		nnode->var = ft_strdup(avzero);
		nnode->val = ft_strdup(avone);
		nnode->next = NULL;
		ft_lstadd_back_env(&env, nnode);
	}
}
