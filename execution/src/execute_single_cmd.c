/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:36:54 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/03 15:41:59 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			return (perror("minishell"), exit_failure(data), 0);
		}
		else
			return (perror("minishell"), exit_failure(data), 0);
	}
	return (1);
}

void	access_and_execute(char *path, char **commande, t_cmd *data, char **envp)
{
	char	**splitpath;
	char	*joinslash;
	char	*cmd;
	int		i;

	splitpath = mysplit(path, ':');
	if (!splitpath)
		(free(commande), exit_failure(data));
	i = -1;
	while (data->cmd[++i])
		commande[i] = ft_strdup(data->cmd[i]);
	commande[i] = NULL;
	i = -1;
	while (splitpath[++i])
	{
		joinslash = ft_strjoin(splitpath[i], "/");
		cmd = ft_strjoin(joinslash, data->cmd[0]);
		free(joinslash);
		if (access(cmd, X_OK | F_OK) == 0)
			execve(cmd, commande, envp);
	}
	free_split(splitpath);
	exit (0);
}
void ft_execute_heredoc(char **commande, char *path, t_cmd *data, char **envp)
{
	int		i;
	char	**splitpath;
	char	*joinslash;
	char	*cmd;
	if (data->cmd)
	{
		splitpath = mysplit(path, ':');
		if (!splitpath)
		{
			(free(commande), exit_failure(data));
		}
		i = -1;
		while (data->cmd[++i])
			commande[i] = ft_strdup(data->cmd[i]);
		commande[i] = NULL;
		i = -1;
		while (splitpath[++i])
		{
			joinslash = ft_strjoin(splitpath[i], "/");
			cmd = ft_strjoin(joinslash, data->cmd[0]);
			free(joinslash);
			if (access(cmd, X_OK | F_OK) == 0)
				execve(cmd, commande, envp);
		}
		(free_split(commande), invalid_msg(data->cmd, data));
	}
}

int	execute_single_cmd(t_env **env, char **envp, t_cmd *data, t_heredoc *heredoc)
{
	char	*path;
	int		pid;
	char	**commande;
	int		status = 0;

	(void)heredoc;
	if (!data->cmd || !env)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (data->cmd[0][0] == '.' && data->cmd[0][1] == '/' && ft_strcmp("./minishell", data->cmd[0]))
			check_dir_file(&data->cmd[0], data);
		if ((data->files && data->redirection))
			ft_do_redirections(data->files, data->redirection, heredoc);
		commande = malloc(sizeof(char *) * (len_arg(&data->cmd[0]) + 1));
		if (!commande)
			exit_failure(data);
		execute_path(data, envp, commande);
		path = find_env(*env, "PATH");
		if (!path)
			(free(commande), invalid_path(data));
		access_and_execute(path, commande, data, envp);
		(free_split(commande), invalid_msg(data->cmd, data));
	}
	else
	{
		(signal(SIGINT, SIG_IGN), waitpid(pid, &status, 0));
		if(WTERMSIG(status) == SIGINT)
			write(2, "\n", 1);
		if(WTERMSIG(status) == SIGQUIT)
			write(2, "Quit: 3\n", 8);
	}
	return (1);
}
