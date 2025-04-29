/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:59:21 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/29 18:14:10 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_unset(t_exp **exp, t_env **env, char **args)
{
	int i;
	t_env *curr_env;
	t_env *prev_env;
	t_exp *curr_exp;
	t_exp *prev_exp;

	if (len_arg(args) < 2)
		return ;
	i = 1;
	while (args[i])
	{
		prev_env = NULL;
		curr_env = *env;
		while (curr_env != NULL)
		{
			if (ft_strcmp(curr_env->var, args[i]) == 0)
			{
				if (prev_env == NULL)
					*env = curr_env->next;
				else
					prev_env->next = curr_env->next;
				free(curr_env->var);
				free(curr_env->val);
				free(curr_env);
				break;
			}
			prev_env = curr_env;
			curr_env = curr_env->next;
		}
		prev_exp = NULL;
		curr_exp = *exp;
		while (curr_exp != NULL)
		{
			if (ft_strcmp(curr_exp->vr, args[i]) == 0)
			{
				if (prev_exp == NULL)
					*exp = curr_exp->next;
				else
					prev_exp->next = curr_exp->next;
				free(curr_exp->vr);
				free(curr_exp->vl);
				free(curr_exp);
				break;
			}
			prev_exp = curr_exp;
			curr_exp = curr_exp->next;
		}
		i++;
	}
}
