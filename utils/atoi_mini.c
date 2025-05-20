/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:46:01 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/16 17:07:30 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include <limits.h> // For INT_MAX and INT_MIN

int	atoi_ph(char *str, int *sign)
{
	unsigned long long	nb;
	int					i;

	nb = 0;
	i = 0;
	*sign = 1;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
		   str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		if ((*sign == 1 && nb > INT_MAX) || 
			(*sign == -1 && nb > (unsigned long long)INT_MAX + 1))
		{
			return (-1);
		}
		i++;
	}
	return ((int)nb);
}

int mini_atoi(char *str)
{
	int	nb;
	int sign;
	
	nb = atoi_ph(str, &sign);
	if (nb == -1)
		return (-1);
	return (nb * sign);
}