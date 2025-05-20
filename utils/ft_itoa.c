/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:09:20 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/20 17:10:00 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_nblen(long nb)
{
	size_t	len;

	len = 0;
	if (nb < 0)
	{
		len++;
		nb *= -1;
	}
	if (nb == 0)
	{
		len++;
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static char	*zero(char *str)
{
	str[0] = '0';
	str[1] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*itoa;
	long	nb;

	len = ft_nblen(n);
	itoa = (char *) malloc(len * sizeof(char) + 1);
	if (!itoa)
		return (NULL);
	nb = n;
	itoa[len--] = '\0';
	if (nb == 0)
		zero(itoa);
	if (nb < 0)
	{
		itoa[0] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		itoa[len] = (nb % 10) + 48;
		nb /= 10;
		len--;
	}
	return (itoa);
}
