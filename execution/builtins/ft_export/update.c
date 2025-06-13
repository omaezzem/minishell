/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:33:12 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:54:38 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	update_join_env(t_env *env, char *var, char *val)
{
	t_env	*current;
	char	*new_val;

	current = env;
	while (current != NULL)
	{
		if (ft_strcmp(current->var, var) == 0)
		{
			new_val = ft_strjoin(current->val, val);
			free(current->val);
			current->val = new_val;
			return ;
		}
		current = current->next;
	}
}

void	update_join_exp(t_exp *exp, char *var, char *val)
{
	t_exp	*current;
	char	*new_val;

	current = exp;
	while (current != NULL)
	{
		if (ft_strcmp(current->vr, var) == 0)
		{
			new_val = ft_strjoin(current->vl, val);
			free(current->vl);
			current->vl = new_val;
			return ;
		}
		current = current->next;
	}
}

void	update_val_env(t_env *env, char *var, char *val)
{
	t_env	*add;

	if (!var || !val)
		return ;
	add = env;
	while (env != NULL)
	{
		if (ft_strcmp(env->var, var) == 0)
		{
			free (env->val);
			env->val = ft_strdup(val);
			env = add;
			break ;
		}
		env = env->next;
	}
	env = add;
}

void	update_val_exp(t_exp *exp, char *var, char *val)
{
	t_exp	*add;

	add = exp;
	if (!var || !val)
		return ;
	while (exp != NULL)
	{
		if (ft_strcmp(exp->vr, var) == 0)
		{
			free(exp->vl);
			exp->vl = ft_strdup(val);
			exp = add;
			break ;
		}
		exp = exp->next;
	}
	exp = add;
}
