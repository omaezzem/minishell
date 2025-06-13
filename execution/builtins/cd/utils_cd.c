/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:45:32 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 09:22:31 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	add_oldpwd_to_env(t_env *ev, char *oldpwd)
{
	t_env	*nnode;

	if (oldpwd)
	{
		nnode = malloc(sizeof(t_env));
		if (!nnode)
			return ;
		nnode->var = ft_strdup("OLDPWD");
		nnode->val = ft_strdup(oldpwd);
		nnode->next = NULL;
		ft_lstadd_back_env(&ev, nnode);
	}
}

void	add_pwd_to_env(t_env *ev)
{
	t_env	*nnode;
	char	un_pwd[PATH_MAX];

	if (getcwd(un_pwd, PATH_MAX))
	{
		nnode = malloc(sizeof(t_env));
		if (!nnode)
			return ;
		nnode->var = ft_strdup("PWD");
		nnode->val = ft_strdup(un_pwd);
		nnode->next = NULL;
		ft_lstadd_back_env(&ev, nnode);
	}
}

void	update_old(t_env **ev, char *oldpath)
{
	t_env		*add;
	int			i;

	i = 0;
	add = *ev;
	while (add != NULL)
	{
		if (ft_strcmp((add)->var, "OLDPWD") == 0)
		{
			i = 1;
			free((add)->val);
			(add)->val = ft_strdup(oldpath);
			break ;
		}
		(add) = (add)->next;
	}
	if (i == 0)
		add_oldpwd_to_env(*ev, oldpath);
}

void	update_new(t_env **ev, char *newpath)
{
	t_env	*add;
	int		i;

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
		add_pwd_to_env(*ev);
}
