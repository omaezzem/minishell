/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:50:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 09:32:08 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*get_home_path(t_env *env)
{
	char	*path;

	path = find_env(env, "HOME");
	if (!path)
		ft_putstr_fd("cd: HOME not set\n", 2);
	return (path);
}

char	*get_oldpwd_path(t_env *env)
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
		if (!ft_strcmp(args[1], "~") || !ft_strcmp(args[1], "~/")
			|| !ft_strcmp(args[1], "--"))
			return (get_home_path(env));
		if (!ft_strcmp(args[1], "-"))
			return (get_oldpwd_path(env));
		return (args[1]);
	}
	ft_putstr_fd("cd: invalid usage\n", 2);
	return (NULL);
}

static int	handle_getcwd_error(t_env *env, char **args, t_exp *exp)
{
	static char	*pth;
	char		*path;

	if (!ft_strcmp(args[1], ".."))
		pth = ft_strjoin(find_env(env, "PWD"), "/..");
	if (!ft_strcmp(args[1], "."))
		pth = ft_strjoin(find_env(env, "PWD"), "/.");
	update_val_env(env, "PWD", pth);
	update_val_exp(exp, "PWD", pth);
	path = determine_path(env, args, len_arg(args));
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	ft_putstr_fd("cd: error retrieving current directory :getcwd: cannot", 2);
	ft_putstr_fd("access parent directories : No such file or directory\n", 2);
	free(pth);
	return (0);
}

int	ft_cd(t_env *env, char **args, t_exp *exp)
{
	char	newpath[PATH_MAX];
	char	oldpath[PATH_MAX];
	char	*path;
	int		lenargs;

	if (!args[0] || !env)
		return (1);
	lenargs = len_arg(args);
	if (lenargs > 2)
		return (ft_putstr_fd("cd: to many argumants\n", 2), 1);
	if (!getcwd(oldpath, PATH_MAX))
		return (handle_getcwd_error(env, args, exp));
	path = determine_path(env, args, lenargs);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	if (!getcwd(newpath, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving current directory\n", 2), 0);
	(update_old(&env, oldpath), update_new(&env, newpath));
	return (0);
}
