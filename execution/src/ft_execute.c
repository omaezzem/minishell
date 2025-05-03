/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/03 17:29:08 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	len_cmd(char **args)
{
	int	i;
    int n;

	i = 0;
    n = 0;
	while (args[i])
	{
		if (args[i][0] == '|')
			n++;
		i++;
	}
	return n;
}
int ft_execute(t_exp *exp, t_env *env, t_cmd *data, char **args)
{
    if (args)
    {
        if (len_cmd(args) == 0)
        {
        if (ft_strcmp(args[0], "echo") == 0)
        {
            ft_echo(args);
            return (0);
        }
        else if (ft_strcmp(args[0], "cd") == 0)
            return (ft_cd(env, args, data->option));
        else if (ft_strcmp(args[0], "pwd") == 0)
        {
            ft_pwd();
            return (0);
        }
        else if (ft_strcmp(args[0], "export") == 0)
            return (ft_export(exp, env, args));
        else if (ft_strcmp(args[0], "env") == 0)
            return (builtin_env(env, args));
        else if (ft_strcmp(args[0], "exit") == 0)
            ft_exit(data, args);
        else if (ft_strcmp(args[0], "unset") == 0)
            ft_unset(&exp, &env, args);
        else
            return (0);
            } 
    }
	return (1);
}
