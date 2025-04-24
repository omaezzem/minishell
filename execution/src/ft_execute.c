/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/24 10:36:30 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int ft_execute(t_cmd *data, char **env, char **args)
// {
//     t_env   *list;

//     if (!args || !args[0])  // Check if args is NULL or empty
//         return 0;
        
//     list = NULL;
//     ft_create_env(env, &list);
//     if (ft_strcmp(args[0], "cd") == 0)
//     {
//         if (!ft_cd(data, args))
//             return 0;
//     }
// 	if (ft_strcmp(args[0], "env") == 0)
//     {
// 		printf("21\n");
//         if (!builtin_env(&data->env, args))
//             return 0;
//     }
//     return 1;
// }