/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:13:20 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:53:22 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	find_plus(char *var)
{
	int	i;

	i = 0;
	if (!var || !var[0])
		return (0);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (minishell_invalid(var), 0);
	while (var[i] && var[i] != '=' && var[i] != '+')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (minishell_invalid(var), 0);
		i++;
	}
	if ((var[i] == '+') && (var[i + 1] == '='))
		return (1);
	return (0);
}

int	len_alnum_var(char *var)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (var[i])
	{
		if (ft_isalnum(var[i]))
			n++;
		i++;
	}
	return (n);
}

int	find_equal(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '=')
		return (minishell_invalid(str), 0);
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	len_at_first_equal(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i);
}

int	len_equal(char *str)
{
	int	i;
	int	n;

	n = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			n++;
		i++;
	}
	return (n);
}
