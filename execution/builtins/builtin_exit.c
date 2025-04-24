/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 05:58:14 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/24 09:02:28 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_num(char *str)
{
	int	i;

	i = 0;
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
	while (args[i])
		i++;
	return i;
}

void	ft_exit(t_cmd *data, char **args)
{
	int largs;
	int	modulo;

	modulo = 0;
	largs = len_arg(args);
	if (!is_num(args[1]))
		printf("exit\nexit: %s: numeric argument required\n", args[1]);
	if (is_num(args[1]) && largs > 2)
	{
		data->ex_status = 1;
		printf("exit\nexit: too many argumants\n");
	}
	else if(is_num(args[1]))
	{
		if (mini_atoi(args[1]) > 256)
		{
			modulo = mini_atoi(args[1]) % 256;
			data->ex_status = modulo;
			printf("exit\n");
			exit(data->ex_status);
		}
		data->ex_status = mini_atoi(args[1]);
		printf("exit\n");
		exit(data->ex_status);
	}
}
