/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:23:25 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:43:41 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_plus_case(t_exp *exp, t_env *env, char *arg, char *avzero)
{
	char	*avone;

	avone = args_one(arg);
	if (!avone)
		avone = ft_strdup("");
	if (search_var_in_exp(exp, avzero))
		update_join_exp(exp, avzero, avone);
	else
		add_to_export_list_vl(&exp, avzero, avone);
	if (search_var_in_env(env, avzero))
		update_join_env(env, avzero, avone);
	else
		add_to_env_list(env, arg, avzero, avone);
	if (ft_strcmp(avone, "") == 0)
		free(avone);
}

void	handle_equal_case(t_exp *exp, t_env *env, char *arg, char *avzero)
{
	char	*avone;

	avone = args_one(arg);
	if (!avone)
		avone = ft_strdup("");
	add_to_export_list_vl(&exp, avzero, avone);
	add_to_env_list(env, arg, avzero, avone);
	if (ft_strcmp(avone, "") == 0)
		free(avone);
}

int	process_export_args(t_exp *exp, t_env *env, char **args)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		if (!process_single_arg(exp, env, args[i]))
		{
			if (len_arg(args) == 2)
				return (0);
		}
		i++;
	}
	return (1);
}

void	print_export_list(t_exp *exp)
{
	t_exp	*curexp;

	curexp = exp;
	while (curexp != NULL)
	{
		if (curexp->vr && !curexp->vl && (ft_strcmp(curexp->vr, "_")))
			printf("declare -x %s\n", curexp->vr);
		else if (ft_strcmp(curexp->vr, "_"))
		{
			printf("declare -x %s=", curexp->vr);
			if (curexp->vl)
				printf("\"%s\"\n", curexp->vl);
		}
		curexp = curexp->next;
	}
}

int	ft_export(t_exp *exp, t_env *env, char **args)
{
	int	len;

	len = len_arg(args);
	if (len > 1 && !process_export_args(exp, env, args))
		return (0);
	if (!if_double_var(&exp))
		return (0);
	if (!sort_exp_list(&exp))
		return (0);
	if ((len_arg(args) == 1) && !ft_strcmp(args[0], "export"))
		print_export_list(exp);
	return (1);
}
