/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dir_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:53:31 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/28 19:53:43 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool is_dir(const char *path)
{
	struct stat path_stat;
	return (stat(path, &path_stat) == 0) && S_ISDIR(path_stat.st_mode);
}

bool is_file(const char *path)
{
	struct stat path_stat;
	return (stat(path, &path_stat) == 0) && S_ISREG(path_stat.st_mode);
}

int     check_dir_file(char **args)
{
	if (is_dir(args[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": is a directory\n", 2);
		return 1;
	}
	if (is_file(args[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return 1;
	}
	return 0;
}
