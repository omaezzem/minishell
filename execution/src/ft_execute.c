/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/03 15:37:57 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_pipes(int (*pipes)[2], t_cmd *data, int numcmd)
{
		if (numcmd > 1 && pipes != NULL)
			free(pipes);
	   	exit_failure(data);	
}

int		create_pipes(int numcmd, int (*pipes)[2], int *pids, t_cmd *data)
{
	int i;

	if (numcmd <= 1)
		return (1);
	i = -1;
	while (++i < (numcmd - 1))
	{
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), free(pids), free_pipes(pipes, data, numcmd), 0);
	}
	return (1);
}

int dup_and_close_pipes(int (*pipes)[2], int i, int numcmd)
{
    int j;

    if (i > 0 && pipes[i-1][0] != -1) {
        if (dup2(pipes[i-1][0], STDIN_FILENO) == -1)
            return (perror("dup2"), 0);
        close(pipes[i-1][0]);
        pipes[i-1][0] = -1; // Mark as closed
    }
    if (i < numcmd - 1 && pipes[i][1] != -1) {
        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
            return (perror("dup2"), 0);
        close(pipes[i][1]);
        pipes[i][1] = -1; // Mark as closed
    }
    j = -1;
    while (++j < numcmd - 1) {
        if (pipes[j][0] != -1) close(pipes[j][0]);
        if (pipes[j][1] != -1) close(pipes[j][1]);
        pipes[j][0] = pipes[j][1] = -1;
    }
    return 1;
}

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

void	ft_close_wait(int numcmd, int (*pipes)[2], int *pids)
{
	int i;
	int status;

	i = -1;
	while (++i < numcmd - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	i = -1;
	while (++i < numcmd)
		waitpid(pids[i], &status, 0);
	signal(SIGINT, SIG_IGN);
	if(WTERMSIG(status) == SIGINT)
		write(2, "\n", 1);
	if(WTERMSIG(status) == SIGQUIT)
		write(2, "Quit: 3\n", 8);
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
		cmd = ft_strjoin(joinslash, curr->cmd[0]);
		free(joinslash);
		if (access(cmd, X_OK | F_OK) == 0)
			(execve(cmd, commande, envp), free(cmd), exit_failure(data));
		free(cmd);
	}
	free_split(commande);
	free_split(splitpath);
	return 1;
}

int	ft_execve(t_cmd *curr, t_env *env, t_cmd *data, char **envp, int i)
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
		path = find_env(env, "PATH");
		if (!path)
			(free(commande), invalid_path(data));
		join_execve_cmd(path, commande, curr, envp, data);
	}
	return 1;
}

int execute_multi_pipe(t_env *env, t_cmd *data, int numcmd, char **envp, t_exp *exp)
{
	int		(*pipes)[2];
	int		*pids;
	int		i;
	t_cmd	*curr;

	pipes = NULL;
	if (numcmd > 1)
	{
		pipes = malloc((numcmd - 1) * (sizeof(*pipes)));
		if (!pipes)
			(perror("malloc"), exit_failure(data));
	}
	pids = malloc(numcmd * sizeof(int));
	if (!pids)
		(perror("malloc"), free_pipes(pipes, data, numcmd));
	curr = data;
	create_pipes(numcmd, pipes, pids, data);
	i = -1;
	while (++i < numcmd && curr)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (perror("fork"), exit_failure(data), 0);
		if (pids[i] == 0)
		{
			if (!curr->cmd || !curr->cmd[0])
			{
				data->ex_status = 127;
                exit(127);
            }
			if (!dup_and_close_pipes(pipes, i, numcmd))
                exit_failure(data);
			if (curr->files && curr->redirection)
				ft_do_redirections(curr->files, curr->redirection, data->heredoc);
			if (curr->cmd && is_builtin(curr->cmd[0]))
			{
				ft_builtins(&env, &exp, curr, data->heredoc);
				curr = curr->next; 
				i++;
				exit_success(data);
			}
			else
				(ft_execve(curr, env, data, envp, i), invalid_msg(curr->cmd, data));
		}
		curr = curr->next;
	}
	ft_close_wait(numcmd, pipes, pids);
	return (1);
}

int ft_execute(t_exp **exp, t_env **env, t_cmd *data, char **envp, t_heredoc *heredoc)
{
	int numcmd;

	int stdin = dup(0);
	if(!data->cmd && data->files && data->redirection)
		ft_do_redirections(data->files, data->redirection, heredoc);
	if (data)
		numcmd = len_cmd(data);
	if (notpipe(data) == 1)
	{
		if (is_builtin(data->cmd[0]))
			ft_builtins(env, exp, data, heredoc);
		else
			execute_single_cmd(env, envp, data, heredoc);
	}
	else
	{
		execute_multi_pipe(*env, data, numcmd, envp, *exp);
	}
	dup2(stdin, 0);
	close(stdin);
	return (1);
}
