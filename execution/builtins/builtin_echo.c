/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:14:24 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/19 09:46:33 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int  len_argsone(char *args)
{
	int i;

	i = 1;
	if (args[0] == '-' && args[1] == 'n')
	{
		i++;
		while (args[i] && args[i] == 'n')
		{
			if (args[i] != 'n')
				return i;
			i++;
		}
	}
	return i;
}

int		ft_echo(char **args)
{
	int i;
	int j;
	int k;

	if (!args)
		return 0;
	j = 1;
	k = 0;
	while (args[j])
	{
		if (len_argsone(args[j]) != ft_strlen(args[j]))
			break;
		k++;
		j++;
	}
	if (k == 0)
	{
		i = k + 1;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] != NULL)
				ft_putstr_fd(" ", 1);
			i++;
		}
		printf("\n");
	}
	else if (k >= 1)
	{
		i = k + 1;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] != NULL)
				ft_putstr_fd(" ", 1);
			i++;
		}
	}
	return 1;
}
