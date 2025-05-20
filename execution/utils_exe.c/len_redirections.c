/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:09:44 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/12 14:56:29 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_len_redirections(char **redirections)
{
    int i;

    i = 0;
    while (redirections[i])
    {
        i++;
    }
    return i;
}
