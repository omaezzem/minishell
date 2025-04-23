/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/23 09:43:31 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int changedir(t_cmd *data, char *path)
{
    char cwd[PATH_MAX];

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
	int		i;
	t_env	*add;

	add = ev;
	i = 0;
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
	int		i;
	t_env	*add;

	add = ev;
	i = 0;
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
	int	res;
	int	old;
	char	newpath[PATH_MAX];
	char	oldpath[PATH_MAX];
	char	*path;
	int		len;

	len = len_args(args);
	if (len > 2)
	{
		if (!getcwd(oldpath, PATH_MAX))
		{
			perror("cd: error retrieving current directory");
			return (0);
		}
		if (ft_strcmp(data->option[0], "~") || data->option[0] == NULL)
		{
			path = find_env(&data->env, "HOME");
			if (!path)
			{
				perror("cd: HOME not set");
				return (0);
			}
		}
		else if (ft_strcmp(data->option[0], "-"))
		{
			path = find_env(&data->env, "OLDPWD");
			if (!path)
			{
				perror("cd: OLDPWD not set");
				return (0);
			}
			printf("%s\n", path);
		}
		else
			path = data->option[1];
		if (!changedir(data, path))
			return (0);
		if (!getcwd(newpath, PATH_MAX))
		{
			perror("cd: error retrieving current directory");
			return (0);
		}
		update_old(data, old);
		update_new(data, newpath);
	}
	else 
	{
		perror("cd: too many arguments");
		return 0;
	}
	return 1;
}
