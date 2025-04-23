/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:24:17 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/22 11:46:49 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_isspace(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            return 1;
        i++;
    }
    return 0;
}
