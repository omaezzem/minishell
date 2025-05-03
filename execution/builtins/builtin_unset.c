/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:59:21 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/03 18:29:45 by omaezzem         ###   ########.fr       */
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
				t_env *to_free = curr_env;
				if (prev_env == NULL)
					*env = curr_env->next;
				else
					prev_env->next = curr_env->next;
				curr_env = curr_env->next;
				free(to_free->var);
				free(to_free->val);
				free(to_free);
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
				t_exp *to_free = curr_exp;
				if (prev_exp == NULL)
					*exp = curr_exp->next;
				else
					prev_exp->next = curr_exp->next;
				curr_exp = curr_exp->next;
				free(to_free->vr);
				free(to_free->vl);
				free(to_free);
				break;
			}
			prev_exp = curr_exp;
			curr_exp = curr_exp->next;
		}

		i++;
	}
}

// int main(int ac, char **av, char env)
// {
// 	int i;
// 	t_env *ev;
// 	t_exp *exp;

// 	env = NULL;
// 	exp = NULL;
// 	exp = ft_create_env_export(env, &exp);
// 	ev = ft_create_env(env, &ev);
// 	ft_unset(&exp, &ev, av);
// 	while (env)
// 	{
// 		printf("%s=%s\n", ev->var, ev->val);
		
// 	}
// }