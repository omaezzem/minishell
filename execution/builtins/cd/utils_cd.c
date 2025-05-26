/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:45:32 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/26 09:55:46 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	update_old(t_env *ev, char *oldpath)
{
	t_env	*add;

	add = ev;
	while (ev != NULL)
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
	while (ev != NULL)
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

static char	*get_home_path(t_env *env)
{
	char	*path;

	path = find_env(env, "HOME");
	if (!path)
		ft_putstr_fd("cd: HOME not set\n", 2);
	return (path);
}

static char	*get_oldpwd_path(t_env *env)
{
	char	*path;

	path = find_env(env, "OLDPWD");
	if (!path)
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
	else
	{
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
	}
	return (path);
}

char	*determine_path(t_env *env, char **args, int lenargs)
{
	if (lenargs == 1)
		return (get_home_path(env));
	if (lenargs == 2)
	{
		if (!ft_strcmp(args[1], "~") || !ft_strcmp(args[1], "~/"))
			return (get_home_path(env));
		if (!ft_strcmp(args[1], "-"))
			return (get_oldpwd_path(env));
		return (args[1]);
	}
	ft_putstr_fd("cd: invalid usage\n", 2);
	return (NULL);
}
