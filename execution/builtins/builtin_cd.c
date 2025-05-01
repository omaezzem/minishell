/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/01 17:45:41 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int changedir(char *path)
{
    if (chdir(path) != 0)
    {
        perror("cd");
        return (0);
    }
    return (1);
}

int len_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
		i++;
	return i;
}
void	update_old(t_env *ev, char *oldpath)
{

	t_env	*add;

	add = ev;
	while (ev->next != NULL)
	{
		if (ft_strcmp(ev->var, "OLDPWD") == 0)
		{
			free(ev->val);
			ev->val = ft_strdup(oldpath);
			ev = add;
			break ;
		}
		ev = ev->next;
	}
	ev = add;
}
void	update_new(t_env *ev, char *newpath)
{
	t_env	*add;

	add = ev;
	while (ev->next != NULL)
	{
		if (ft_strcmp(ev->var, "PWD") == 0)
		{
			free(ev->val);
			ev->val = ft_strdup(newpath);
			ev = add;
			break ;
		}
		ev = ev->next;
	}
	ev = add;
}

int	ft_cd(t_cmd *data, char **args)
{
	char	newpath[PMAX];
	char	oldpath[PMAX];
	char	*path;
	path = NULL;
	int		len;

	len = len_args(args);
	if (len <= 2)
	{
		if (!getcwd(oldpath, PATH_MAX))
		{
			ft_putstr_fd("cd: error retrieving current directory", 2);
			return (0);
		}
		if (data->cmd[0] && (!data->cmd[1] || !data->option[0]))
		{
			if (!ft_strcmp(data->cmd[0], "cd"))
			{
				path = find_env(data->env, "HOME");
				printf("%s\n", path);
				if (!path)
				{
					perror("cd: HOME not set");
					return (0);
				}
			}
		}
		else if (data->cmd[1] && data->cmd[0] && !data->cmd[2])
		{
			if (!ft_strcmp(data->option[0], "~") && (!ft_strcmp(data->cmd[0], "cd")))
			{
				path = find_env(data->env, "HOME");
				if (!path)
				{
					perror("cd: HOME not set");
					return (0);
				}
			}
		}
		else if (data->option[0] && data->cmd[0] && !data->cmd[2])
		{
			if (!ft_strcmp(data->option[0], "-") && (!ft_strcmp(data->cmd[0], "cd")))
			{
				path = find_env(data->env, "OLDPWD");
				if (!path)
				{
					ft_putstr_fd("cd: OLDPWD not set\n", 2);
					return (0);
				}
				printf("%s\n", path);
			}
		}
		else
			path = data->cmd[1];
		if (!changedir(path))
			return (0);
		if (!getcwd(newpath, PATH_MAX))
		{
			ft_putstr_fd("cd: error retrieving current directory", 2);
			return (0);
		}
		update_old(data->env, oldpath);
		update_new(data->env, newpath);
	}
	else
	{
		ft_putstr_fd("cd: too many arguments", 2);
		return 0;
	}
	return 1;
}
