/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 05:58:14 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/24 15:50:29 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+') 
		i++;
	if (!str[i])
		return (FAILURE);
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	len_arg(char **args)
{
	int i;

	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	return (i);
}
static int	calculate_exit_code(int exit_code)
{
	int	modulo;

	if (exit_code > 255)
		modulo = exit_code % 256;
	else if (exit_code < 0)
		modulo = exit_code + 256;
	else
		modulo = exit_code;
	return (modulo);
}

static void	handle_exit_error(t_cmd *data, char *arg)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	data->ex_status = 255;
	exit(255);
}

void	ft_exit(t_cmd *data, char **args)
{
	int	largs;
	int	exit_code;

	if (!args)
		return ;
	largs = len_arg(args);
	if (largs > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		data->ex_status = 1;
		return ;
	}
	if (largs == 2)
	{
		if (is_num(args[1]) == FAILURE)
			handle_exit_error(data, args[1]);
		exit_code = calculate_exit_code(mini_atoi(args[1]));
		data->ex_status = exit_code;
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(exit_code);
	}
	data->ex_status = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	exit(0);
}
