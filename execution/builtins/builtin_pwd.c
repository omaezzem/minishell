/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 02:14:59 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/27 13:55:48 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*path;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return(printf("%s\n", cwd), 1);
	else
	{
		path = find_env(env, "PWD");
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		return 0;
	}
	return 1;
}
