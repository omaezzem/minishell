/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:45:32 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/01 18:26:08 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	update_old(t_env **ev, char *oldpath)
{
	t_env	*add;

	add = *ev;
	while (add != NULL)
	{
		if (ft_strcmp((add)->var, "OLDPWD") == 0)
		{
			free((add)->val);
			(add)->val = ft_strdup(oldpath);
			break ;
		}
		(add) = (add)->next;
	}
}

void	update_new(t_env **ev, char *newpath)
{
	t_env	*add;
	char	un_pwd[PATH_MAX];
	int		i;
	t_env	*nnode;

	i = 1;
	add = *ev;
	while (add != NULL)
	{
		if (ft_strcmp(add->var, "PWD") == 0)
		{
			i = 0;
			free(add->val);
			add->val = ft_strdup(newpath);
			break ;
		}
		add = (add)->next;
	}
	if (i == 1)
	{
		if (getcwd(un_pwd, PATH_MAX))
		{
			nnode = malloc(sizeof(t_env));
			if (!nnode)
				return ;
			nnode->var = ft_strdup("PWD");
			nnode->val = ft_strdup(un_pwd);
			nnode->next = NULL;
			ft_lstadd_back_env(ev, nnode);
		}
	}
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
