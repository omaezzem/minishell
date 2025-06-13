/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:22:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/13 15:22:00 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long ft_atoi(const char *str)
{
    int         i;
    long long   sign;
    long long   result;
    long long   prev;

    i = 0;
    sign = 1;
    result = 0;

    while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
    {
        prev = result;
        result = result * 10 + (str[i] - '0');        
        if (result < prev)
        {
            if (sign == 1)
                return (LLONG_MAX);
            else
                return (LLONG_MIN);
        }
        i++;
    }
    return (result * sign);
}
