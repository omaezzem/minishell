/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/20 15:27:51 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main(int ac, char **av, char **env)
{
    t_env   *ev;

    ev = NULL;
    (void)ac;
    (void)av;
    ft_create_env(env, &ev);
}