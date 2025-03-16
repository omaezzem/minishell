/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:14:24 by omaezzem          #+#    #+#             */
/*   Updated: 2025/03/14 06:05:17 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_mins_n(char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		if (!ft_strcmp(args[i], "-n"))
			return 0;
		i++;
	}
	return 1;
}

int    options_echo_pos(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (args[i] && !ft_strcmp(args[i], "-n"))
	{
		i++;
	}
	return i;
}

void	ft_echo(char **args)
{
	int i;

	i = options_echo_pos(args);
	if (check_mins_n(args))
	{
		while (args[i])
		{
			printf("%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
		printf("\n");
	}
	else if (check_mins_n(args) == 0)
	{
		while (args[i])
		{
			printf("%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
}
