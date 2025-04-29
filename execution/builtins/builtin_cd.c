/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/29 16:48:38 by omaezzem         ###   ########.fr       */
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
	int		len;

	len = len_args(args);
	if (len <= 2)
	{
		if (!getcwd(oldpath, PATH_MAX))
		{
			ft_putstr_fd("cd: error retrieving current directory", 2);
			return (0);
		}
		if ((ft_strcmp(data->option[0], "~") == 0) || data->option[0] == NULL)
		{
			path = find_env(&data->env, "HOME");
			if (!path)
			{
				perror("cd: HOME not set");
				return (0);
			}
		}
		else if (ft_strcmp(data->option[0], "-") == 0)
		{
			path = find_env(&data->env, "OLDPWD");
			if (!path)
			{
				ft_putstr_fd("cd: OLDPWD not set", 2);
				return (0);
			}
			printf("%s\n", path);
		}
		else
			path = data->option[1];
		if (!changedir(path))
			return (0);
		if (!getcwd(newpath, PATH_MAX))
		{
			ft_putstr_fd("cd: error retrieving current directory", 2);
			return (0);
		}
		update_old(&data->env, oldpath);
		update_new(&data->env, newpath);
	}
	else
	{
		ft_putstr_fd("cd: too many arguments", 2);
		return 0;
	}
	return 1;
}

// int main(int ac, char **av, char **env)
// {
//     t_env    *list;
// 	t_cmd	*cmd;

// 	cmd = NULL;
//     list = NULL;
//     char *input;
//     list = ft_create_env(env, &list);

//     while (1)
//     {
//         input = readline("minishell-0.0$ ");
//         if (input == NULL)
//             return (1);
//         if(*input == '\0')
//             break;

//         char **args = ft_split(input, ' ');

//         ft_cd(cmd ,args);

//         char *pwd =find_env(list, "PWD");

//         printf("\nCurrent directory: %s\n\n", pwd);
//     }
// }
