/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/04 17:27:47 by omaezzem         ###   ########.fr       */
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

int ft_builtins(char **args, t_env *env, t_exp *exp, t_cmd *data)
{
    if (ft_strcmp(args[0], "echo") == 0)
    {
        ft_echo(args);
        return (1);
    }
    else if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(env, args, data->option), 1);
    else if (ft_strcmp(args[0], "pwd") == 0)
    {
        ft_pwd();
        return (1);
    }
    else if (ft_strcmp(args[0], "export") == 0)
        return (ft_export(exp, env, args), 1);
    else if (ft_strcmp(args[0], "env") == 0)
        return (builtin_env(env, args), 1);
    else if (ft_strcmp(args[0], "exit") == 0)
        return (ft_exit(data, args), 1);
    else if (ft_strcmp(args[0], "unset") == 0)
        return (ft_unset(&exp, &env, args), 1);
    else
        return (0);
}
int is_builtin(char **args)
{
    if (!ft_strcmp(args[0], "echo")
        || !ft_strcmp(args[0], "export")
        || !ft_strcmp(args[0], "env")
        || !ft_strcmp(args[0], "unset")
        || !ft_strcmp(args[0], "exit")
        || !ft_strcmp(args[0], "pwd")
        || !ft_strcmp(args[0], "cd"))
        return 1;
    return 0;
}
int ft_execute(t_exp *exp, t_env *env, t_cmd *data, char **args)
{
    if (args)
    {
        if (len_cmd(args) == 0)
        {
            if (is_builtin(args))
                ft_builtins(args, env, exp, data);
        }
    }
	return (1);
}
