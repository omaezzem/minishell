/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/09 22:39:06 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	changedir(char *path)
{
	if (chdir(path) != 0)
		return (perror("cd"), 0);
	return (1);
}

int	ft_cd(t_env *env, char **args ,t_exp *exp)
{
	char		newpath[PATH_MAX];
	char		oldpath[PATH_MAX];
	char		*path;
	int			lenargs;
	static char	*pth;

	if (!args[0] || !env)
		return (1);
	lenargs = len_arg(args);
	if (!getcwd(oldpath, PATH_MAX))
	{
		if (!ft_strcmp(args[1], ".."))
			pth = ft_strjoin(find_env(env, "PWD"), "/..");
		if (!ft_strcmp(args[1], "."))
			pth = ft_strjoin(find_env(env, "PWD"), "/.");
		update_val_env(env, "PWD", pth);
		update_val_exp(exp, "PWD", pth);
		path = determine_path(env, args, lenargs);
		if (!changedir(path))
			return (1);
		return (ft_putstr_fd("cd: error retrieving current directory : getcwd: cannot access parent directories : No such file or directory\n", 2), 0);
	}
	path = determine_path(env, args, lenargs);
	if (!path)
		return (1);
	if (!changedir(path))
		return (1);
	if (!getcwd(newpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	update_old(&env, oldpath);
	update_new(&env, newpath);
	return (0);
}
