/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:36:54 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 15:15:48 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			free_split(splitpath), execve(cmd, commande, envp);
		free(cmd);
	}
	if (splitpath)
		free_split(splitpath);
}

static void	handle_child_signals(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

static void	prepare_and_execute(t_cmd *data, t_env **env, char **envp, int herdc)
{
	char	**commande;
	char	*path;

	if (data->files && data->redirection)
		ft_do_redirections(data->files, data->redirection, herdc);
	commande = malloc(sizeof(char *) * (len_arg(&data->cmd[0]) + 1));
	if (!commande)
		exit_failure(data);
	execute_path(data, envp, commande);
	path = find_env(*env, "PATH");
	if (!path)
	{
		free(commande);
		invalid_path(data);
	}
	access_and_execute(path, commande, data, envp);
	free_split(commande);
	invalid_msg(data->cmd, data);
}

static void	handle_parent_process(int status)
{
	if (WTERMSIG(status) == SIGINT)
		write(2, "\n", 1);
	if (WTERMSIG(status) == SIGQUIT)
		write(2, "Quit: 3\n", 8);
}

int	execute_single_cmd(t_env **env, char **envp, t_cmd *data, t_heredoc *heredoc)
{
	int	pid;
	int	herdc;
	int status;

	herdc = heredoc->fd;
	if (!data->cmd || !env)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		handle_child_signals();
		if (data->cmd[0][0] == '.' && data->cmd[0][1] == '/'
			&& ft_strcmp("./minishell", data->cmd[0])
			&& access(data->cmd[0], X_OK | F_OK))
			check_dir_file(&data->cmd[0], data);
		prepare_and_execute(data, env, envp, herdc);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		handle_parent_process(status);
		update_exit_status(status, data);
	}
	return (1);
}
