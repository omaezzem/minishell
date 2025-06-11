/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excptwo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:58:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 23:51:41 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		is_cmd_path(t_cmd *data, t_cmd *curr, char **commande, char **envp, int i)
{
	int	j;

	j = -1;
	if (data->cmd && curr->cmd && curr->cmd[0])
	{
		if (ft_strchr(data->cmd[0] , '/'))
		{
			if (access(curr->cmd[0], X_OK | F_OK) == 0)
			{
				while (curr->cmd[++j])
					commande[j] = ft_strdup(curr->cmd[j]);
				commande[j] = NULL;
				execve(curr->cmd[i], commande, envp);
			}
		}
	}
	return (1);
}

int join_execve_cmd(char *path, char **commande, t_cmd *curr, char **envp, t_cmd *data)
{
	char **splitpath;
	char *joinslash;
	char *cmd;
	int j;

	splitpath = mysplit(path, ':');
	if (!splitpath)
		(free(commande), exit_failure(data));
	j = -1;
	while (splitpath[++j])
	{
		joinslash = ft_strjoin(splitpath[j], "/");
		if (!joinslash)
            (free_split(splitpath), free_split(commande), exit_failure(data));
		cmd = ft_strjoin(joinslash, curr->cmd[0]);
		free(joinslash);
		if (!cmd) 
            free_split(splitpath), free_split(commande), exit_failure(data);
		if (access(cmd, X_OK | F_OK) == 0)
			(free(splitpath), execve(cmd, commande, envp), free(cmd),
				free_split(commande), exit_failure(data));
		free(cmd);
	}
	return (free_split(commande), free_split(splitpath), 1);
}

int	ft_execve(t_cmd *curr, t_env **env, t_cmd *data, char **envp, int i)
{
	int		lencmd;
	char	*path;
	char	**commande;
	int		j;

	if (curr->cmd)
	{
		lencmd = 0;
		j = -1;
		while (curr->cmd[++j])
			lencmd += len_arg(&curr->cmd[j]);
		commande = malloc(sizeof(char *) * (lencmd + 1));
		if (!commande)
			 exit_failure(data);
		is_cmd_path(data, curr, commande, envp, i);
		j = -1;
		while (curr->cmd[++j])
			commande[j] = ft_strdup(curr->cmd[j]);
		commande[j] = NULL;
		path = find_env(*env, "PATH");
		if (!path)
			(free(commande), invalid_path(data));
		join_execve_cmd(path, commande, curr, envp, data);
	}
	return 1;
}

int	execute_path(t_cmd *data, char **envp, char **commande)
{
	int	i;

	i = -1;
	if (ft_strchr(data->cmd[0], '/'))
	{
		if (access(data->cmd[0], X_OK | F_OK) == 0)
		{
			while (data->cmd[++i])
				commande[i] = ft_strdup(data->cmd[i]);
			commande[i] = NULL;
			execve(data->cmd[0], commande, envp);
			return (perror("minishell"), free_split(commande), exit_failure(data), 0);
		}
		else
			return (perror("minishell"), exit_failure(data), 0);
	}
	return (1);
}