/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/25 18:25:14 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_execute(t_cmd *data, char **args, char **env)
{
	if (!args || !args[0])
		return (0);
	
	if (strcmp(args[0], "echo") == 0)
    {
		ft_echo(args);
        return (0);
    }
	else if (strcmp(args[0], "cd") == 0)
		return (ft_cd(data, args));
	else if (strcmp(args[0], "pwd") == 0)
    {
		ft_pwd();
        return (0);
    }
	else if (strcmp(args[0], "export") == 0)
		return (ft_export(&data->env, args, env));
	else if (strcmp(args[0], "env") == 0)
		return (builtin_env(&data->env, args));
	else if (strcmp(args[0], "exit") == 0)
		ft_exit(data, args);
	else
		return (0);
	return (1);
}