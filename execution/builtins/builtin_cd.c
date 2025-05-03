/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/03 13:37:57 by omaezzem         ###   ########.fr       */
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
	char	*path;
	int		lenargs;
	int		lenopt;

	lenargs = len_args(args);
	printf("%s\n", args[0]);
	if (options)
		lenopt = len_args(options);
	if (!getcwd(oldpath, PATH_MAX))
	{
		ft_putstr_fd("cd: error retrieving current directory", 2);
		return (0);
	}
	if (lenargs == 1 && lenopt == 0)
	{
		path = find_env(env, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (0);
		}
	}
	else if (lenargs == 2 && lenopt == 0)
	{
		if (ft_strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "~/") == 0)
		{
			path = find_env(env, "HOME");
			if (!path)
			{
				ft_putstr_fd("cd: HOME not set\n", 2);
				return (0);
			}
		}
		else
		{
			path = args[1];
		}
	}
	else if (lenargs == 1 && lenopt == 1 && ft_strcmp(options[0], "-") == 0)
	{
		path = find_env(env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (0);
		}
		printf("%s\n", path);
	}
	else
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (0);
	}
	if (!changedir(path))
		return (0);
	if (!getcwd(newpath, PATH_MAX))
	{
		ft_putstr_fd("cd: error retrieving current directory", 2);
		return (0);
	}
	update_old(env, oldpath);
	update_new(env, newpath);
	return (1);
}
