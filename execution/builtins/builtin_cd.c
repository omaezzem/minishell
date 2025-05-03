/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/03 17:52:05 by omaezzem         ###   ########.fr       */
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

int	ft_cd(t_env *env, char **args, char **options)
{
	char	newpath[PATH_MAX];
	char	oldpath[PATH_MAX];
	char	*path = NULL;
	int		lenargs = len_args(args);
	int		lenopt;
	if (options)
		lenopt = len_args(options);
	else
		lenopt = 0;

	if (!getcwd(oldpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	if (lenargs == 1)
	{
		if (options == NULL || lenopt == 0)
		{
			path = find_env(env, "HOME");
			if (!path)
				return (ft_putstr_fd("cd: HOME not set\n", 2), 0);
		}
		else if (lenopt == 1)
		{
			if (!ft_strcmp(options[0], "~")
				|| !ft_strcmp(options[0], "~/"))
			{
				path = find_env(env, "HOME");
				if (!path)
					return (ft_putstr_fd("cd: HOME not set\n", 2), 0);
			}
			else if (!ft_strcmp(options[0], "-"))
			{
				path = find_env(env, "OLDPWD");
				if (!path)
					return (ft_putstr_fd("cd: OLDPWD not set\n", 2), 0);
				printf("%s\n", path);
			}
		}
	}
	else if (lenargs >= 2)
		path = args[1];
	else
		return (ft_putstr_fd("cd: invalid usage\n", 2), 0);
	if (!path)
		return (ft_putstr_fd("cd: no path provided\n", 2), 0);
	if (!changedir(path))
		return (0);
	if (!getcwd(newpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	update_old(env, oldpath);
	update_new(env, newpath);
	return (1);
}
