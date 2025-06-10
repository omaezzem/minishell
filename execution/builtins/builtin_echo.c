/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:14:24 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/09 22:38:03 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	len_argsone(char *args)
{
	int	i;

	i = 0;
	if (args[0] == '-' && args[1] == 'n')
	{
		i = 1;
		i++;
		while (args[i] && args[i] == 'n')
		{
			if (args[i] != 'n')
				return (i);
			i++;
		}
	}
	return (i);
}

void	print_echo(int k, char **args)
{
	int	i;

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
}

int	ft_echo(char **args)
{
	int	j;
	int	k;

	if (!args)
		return (1);
	j = 1;
	k = 0;

	while (args[j])
	{
		if (len_argsone(args[j]) != ft_strlen(args[j]))
			break ;
		k++;
		j++;
	}
	print_echo(k, args);
	return (0);
}
