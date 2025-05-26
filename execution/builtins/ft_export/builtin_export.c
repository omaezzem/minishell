/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:23:25 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/26 15:33:19 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_plus_case(t_exp *exp, t_env *env, char *arg, char *avzero)
{
	char	*avone;

	avone = args_one(arg);
	if (!avone)
		return (free(avzero));
	if (search_var_in_exp(exp, avzero))
		update_join_exp(exp, avzero, avone);
	else
		add_to_export_list_vl(&exp, avzero, avone);
	if (search_var_in_env(env, avzero))
		update_join_env(env, avzero, avone);
	else
		add_to_env_list(env, arg, avzero, avone);
	free(avzero);
}

void	handle_equal_case(t_exp *exp, t_env *env, char *arg, char *avzero)
{
	char	*avone;

	avone = args_one(arg);
	if (!avone)
		avone = ft_strdup("");
	add_to_export_list_vl(&exp, avzero, avone);
	add_to_env_list(env, arg, avzero, avone);
	free(avzero);
}

void	process_export_args(t_exp *exp, t_env *env, char **args)
{
	int		i;
	char	*avzero;

	i = 1;
	while (args[i])
	{
		if (!args[i])
			continue ;
		if (!ft_isalpha(args[i][0]))
			return (minishell_invalid(args[i]));
		avzero = check_var(args_zero(args[i]));
		if (!avzero)
			return ;
		if (find_plus(args[i]))
			handle_plus_case(exp, env, args[i], avzero);
		else if (find_equal(args[i]))
			handle_equal_case(exp, env, args[i], avzero);
		else
			add_to_export_list_v(&exp, avzero);
		i++;
	}
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

int		ft_export(t_exp *exp, t_env *env, char **args)
{
	int		len;

	len = len_arg(args);
	if (len > 1)
		process_export_args(exp, env, args);
	if_double_var(&exp);
	sort_exp_list(&exp);
	if ((len_arg(args) == 1) && !ft_strcmp(args[0], "export"))
		print_export_list(exp);
	return (0);
}
