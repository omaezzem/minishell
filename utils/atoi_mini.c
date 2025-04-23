/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:46:01 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/22 11:46:42 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	atoi_ph(char *str)
{
	unsigned long long	nb;
	int					i;

	nb = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		if (nb > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)nb);
}

int mini_atoi(char *str)
{
	int	nb;

	nb = atoi_ph(str);
	if (nb == -1 || nb > INT_MAX)
		return (-1);
	return (nb);
}
