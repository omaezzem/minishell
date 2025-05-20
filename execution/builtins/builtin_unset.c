/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:59:21 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/15 21:09:05 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unset_invalid(char *invalid_str)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(invalid_str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int check_args(char *args)
{
	int i;

	i = 0;
	while (args[i])
	{
		if (!ft_isalnum(args[i]))
			return 0;
		i++;
	}
	return 1;
}
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
		if (!check_args(args[i]))
			unset_invalid(args[i]);
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
