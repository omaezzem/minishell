/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exptwo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:27:18 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:56:10 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	determine_export_case(t_exp *exp, t_env *env, char *arg, char *avzero)
{
	if (find_plus(arg))
		handle_plus_case(exp, env, arg, avzero);
	else if (find_equal(arg))
		handle_equal_case(exp, env, arg, avzero);
	else
		add_to_export_list_v(&exp, avzero);
}

int	process_single_arg(t_exp *exp, t_env *env, char *arg)
{
	char	*avzero;

	if (!arg)
		return (true);
	if (!ft_isalpha(arg[0]))
		return (minishell_invalid(arg), 0);
	avzero = check_var(args_zero(arg));
	if (!avzero)
		return (0);
	determine_export_case(exp, env, arg, avzero);
	free(avzero);
	return (1);
}
