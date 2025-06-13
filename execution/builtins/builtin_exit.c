/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 05:58:14 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/13 17:06:38 by omaezzem         ###   ########.fr       */
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
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	return (i);
}

static void	handle_exit_error(t_cmd *data, char *arg, int st)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	data->ex_status = st;
	printf("%d\n", st);
	exit(st);
}

static int	calculate_exit_code(long long exit_code, char *arg, t_cmd *data)
{
	int	modulo;

	if (exit_code == LLONG_MAX || exit_code > INT_MAX)
		handle_exit_error(data, arg, 255);
	if (exit_code == LLONG_MIN || exit_code < INT_MIN)
		handle_exit_error(data, arg, 0);
	if (exit_code > 255)
		modulo = exit_code % 256;
	else if (exit_code < 0)
		modulo = exit_code + 256;
	else
		modulo = exit_code;
	return (modulo);
}

void	ft_exit(t_cmd *data, char **args)
{
	int			largs;
	long long	exit_code;

	if (!args)
		return ;
	largs = len_arg(args);

	if (largs > 2 && is_num(args[1]))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		data->ex_status = 1;
		return ;
	}
	else if (largs > 2 && !is_num(args[1]))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		data->ex_status = 255;
		return ;
	}
	else if (largs == 2)
	{
		if (is_num(args[1]) == FAILURE)
			handle_exit_error(data, args[1], 255);
		exit_code = calculate_exit_code(ft_atoi(args[1]), args[1], data);
		data->ex_status = exit_code;
		(ft_putstr_fd("exit\n", STDERR_FILENO), exit(exit_code));
	}
	else
		(ft_putstr_fd("exit\n", STDERR_FILENO), exit(data->ex_status));
}
