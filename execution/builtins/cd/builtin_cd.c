/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/26 09:58:03 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	changedir(char *path)
{
	if (chdir(path) != 0)
		return (perror("cd"), 0);
	return (1);
}

int	ft_cd(t_env *env, char **args)
{
	char	newpath[PATH_MAX];
	char	oldpath[PATH_MAX];
	char	*path;
	int		lenargs;

	if (!args[0] || !env)
		return (0);
	lenargs = len_arg(args);
	if (!getcwd(oldpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	path = determine_path(env, args, lenargs);
	if (!path)
		return (0);
	if (!changedir(path))
		return (0);
	if (!getcwd(newpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	update_old(env, oldpath);
	update_new(env, newpath);
	return (1);
}
