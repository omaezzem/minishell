/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 05:58:14 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/16 20:32:59 by omaezzem         ###   ########.fr       */
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

void	ft_exit(t_cmd *data, char **args)
{
	int	largs;
	int	exit_code;
	int	modulo;

	if (!args)
		return ;
	largs = len_arg(args);
	if (largs > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->ex_status = 1;
		return;
	}
	if (largs == 2)
	{
		if (!is_num(args[1]))
		{
			ft_putstr_fd("minishell: exit:", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			data->ex_status = 255;
			printf("exit\n");
			exit(255);
		}
		exit_code = mini_atoi(args[1]);
		if (exit_code > 255)
			modulo = exit_code % 256;
		else if (exit_code < 0)
			modulo = exit_code + 256;
		else
			modulo = exit_code;
		data->ex_status = modulo;
		printf("exit\n");
		exit(modulo);
	}
	data->ex_status = 0;
	exit(0);
}
