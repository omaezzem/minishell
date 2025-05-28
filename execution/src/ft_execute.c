/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/28 13:44:11 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int notpipe(t_cmd *data)
{
	if (!data || !data->cmd)
		return 0;
	if (!data->next)
		return 1;
	return 0;
}
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
int ft_builtins(t_env **env, t_exp **exp, t_cmd *data)
{
	if (data->files && data->redirection && notpipe(data))
		ft_do_redirections(data->files, data->redirection);
	if (ft_strcmp(data->cmd[0], "echo") == 0)
		return(ft_echo(&data->cmd[0]), 1);
	else if (ft_strcmp(data->cmd[0], "cd") == 0)
		return (ft_cd(*env, data->cmd, *exp), 1);
	else if (ft_strcmp(data->cmd[0], "pwd") == 0)
		return (ft_pwd(*env), 1);
	else if (ft_strcmp(data->cmd[0], "export") == 0)
		return (ft_export(*exp, *env, data->cmd), 1);
	else if (ft_strcmp(data->cmd[0], "env") == 0)
		return (builtin_env(env, data->cmd), 1);
	else if (ft_strcmp(data->cmd[0], "exit") == 0)
		return (ft_exit(data, data->cmd), 1);
	else if (ft_strcmp(data->cmd[0], "unset") == 0)
		return (ft_unset(exp, env, data->cmd), 1);
	else
		return (0);
}
int is_builtin(char *args)
{
	if (args)
	{
		if (!ft_strcmp(args, "echo")
			|| !ft_strcmp(args, "export")
			|| !ft_strcmp(args, "env")
			|| !ft_strcmp(args, "unset")
			|| !ft_strcmp(args, "exit")
			|| !ft_strcmp(args, "pwd")
			|| !ft_strcmp(args, "cd"))
			return 1;
	}
	return 0;
}

int index_last_outfile(char **files, char **redirections)
{
	int i = 0;
	int s = 0;

	while (files[i] != NULL)
	{
		if(redirections[i][0] == '>')
			s = i;
		i++;
	}
	return s;
}
int index_last_infile(char **files, char **redirections)
{
	int i = 0;
	int s = 0;

	while (files[i] != NULL)
	{
		if(redirections[i][0] == '<')
			s = i;
		i++;
	}
	return s;
}

int len_cmd(t_cmd *data)
{
	int i;
	t_cmd *curr;

	curr = data;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return i;
}

int   ft_do_redirections(char **files, char **redirections)
{
	int fd;
	int i_outfl;
	int i_infl;
	int i;

	if (files)
	{
		i_outfl = index_last_outfile(files, redirections);
		i_infl = index_last_infile(files, redirections);
	}
		
	if (files && redirections)
	{
		if (ft_len_redirections(redirections) == 1)
			to_single_redirection(files, redirections);
		else
		{
			i = 0;
			while (files[i] && i < i_outfl)
			{
				if (ft_strcmp(redirections[i], ">") == 0
					|| ft_strcmp(redirections[i], ">>") == 0)
				{
						ft_output_append(files, redirections, i);
				}
				i++;
			}
			i = 0;
			while (files[i] && i < i_infl)
			{
				if (ft_strcmp(redirections[i], "<") == 0
					|| ft_strcmp(redirections[i], "<<") == 0)
				{
					ft_inp_heredoc(files, redirections, i);
				}
				i++;
			}
			if (files[i_outfl] != NULL)
			{
				if (ft_strcmp(redirections[i_outfl], ">") == 0
					|| ft_strcmp(redirections[i_outfl], ">>") == 0)
				{
					fd = ft_output_append(files, redirections, i_outfl);
					if (dup2(fd, STDOUT_FILENO) == -1)
						return (close(fd), perror("minishell"), FAILURE);
				}
			}
			if (files[i_infl])
			{
				if (ft_strcmp(redirections[i_infl], "<") == 0
					|| ft_strcmp(redirections[i_infl], "<<") == 0)
				{
					fd = ft_inp_heredoc(files, redirections, i_infl);
					if (dup2(fd, STDIN_FILENO) == -1)
						return (close(fd), perror("minishell"), FAILURE);
				}
			}
		}
	}
	return SUCCESS;
}
int execute_multi_pipe(t_env *env, t_cmd *data, int numcmd, char **envp, t_exp *exp)
{
	int pipes[numcmd - 1][2];
	int pids[numcmd];
	int i;
	int j;
	char    **commande;
	char    **splitpath;
	char    *path;
	char    *joinslash;
	char    *cmd;
	t_cmd   *curr;
	int     lencmd;

	i = 0;
	curr = data;
	while (i < (numcmd - 1))
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	if (!curr->cmd)
		curr = curr->next, numcmd--;
	while (i < numcmd && curr)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			if (curr->files && curr->redirection)
				ft_do_redirections(curr->files, curr->redirection);
			if (i > 0)
				dup2(pipes[i-1][0], STDIN_FILENO);
			if (i < numcmd - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			j = 0;
			while (j < numcmd - 1)
			{
				(close(pipes[j][0]), close(pipes[j][1]));
				j++;
			}
			if (is_builtin(curr->cmd[0]))
			{
				ft_builtins(&env, &exp, curr);
				curr = curr->next;
				i++;
				exit(EXIT_SUCCESS);
			}
			else
			{
				lencmd = 0;
				j = 0;
				while (curr->cmd[j])
				{
					lencmd += len_arg(&curr->cmd[j]);
					j++;
				}
				commande = malloc(sizeof(char *) * (lencmd + 1));
				if (!commande)
					exit(EXIT_FAILURE);
				// printf("HELLO\n");
				if (data->cmd)
				{
					if (ft_strchr(data->cmd[0] , '/'))
					{
						if (access(curr->cmd[0], X_OK | F_OK) == 0)
						{
							commande[0] = ft_strdup(curr->cmd[i]);
							commande[1] = NULL;
							execve(curr->cmd[i], commande, envp);
						}
					}
				}
				j = 0;
				while (curr->cmd[j])
				{
					commande[j] = ft_strdup(curr->cmd[j]);
					j++;
				}
				commande[j] = NULL;
				path = find_env(env, "PATH");
				if (!path)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(curr->cmd[i], 2);
					ft_putstr_fd(": command not found\n", 2);
					free(commande);
				}
				splitpath = mysplit(path, ':');
				if (!splitpath)
				{
					free(commande);
					exit(EXIT_FAILURE);
				}
				j = 0;
				while (splitpath[j])
				{
					joinslash = ft_strjoin(splitpath[j], "/");
					cmd = ft_strjoin(joinslash, curr->cmd[0]);
					free(joinslash);
					if (access(cmd, X_OK | F_OK) == 0)
					{
						execve(cmd, commande, envp);
						exit(EXIT_FAILURE);
					}
					free(cmd);
					j++;
				}
				if (curr->cmd[0][0] == '.' && curr->cmd[0][1] == '/' && ft_strcmp(curr->cmd[0], "./minishell"))
					return(check_dir_file(&curr->cmd[0]), 0);
				else
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(curr->cmd[0], 2);
					ft_putstr_fd("  command not found\n", 2);
				}
				free(commande);
				free_split(splitpath);
				exit(EXIT_FAILURE);
			}
		}
		curr = curr->next;
		i++;
	}
	i = 0;
	while (i < numcmd - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	i = 0;
	while (i < numcmd)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	return 1;
}

int execute_path(t_cmd *data, char **envp, char **commande)
{
	if (ft_strchr(data->cmd[0] , '/'))
	{
		if (access(data->cmd[0], X_OK | F_OK) == 0)
		{
			commande[0] = ft_strdup(data->cmd[0]);
			commande[1] = NULL;
			execve(data->cmd[0], commande, envp);
			return (perror("minishell"), exit(EXIT_FAILURE), 0);
		}
		else
			return (perror("minishell"), exit(EXIT_FAILURE), 0);
	}
	return 1;
}

void	invalid_msg(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(EXIT_FAILURE);
}

void	access_and_execute(char *path, char **commande, t_cmd *data, char **envp)
{
	char	**splitpath;
	char	*joinslash;
	char	*cmd;
	int		i;

	splitpath = mysplit(path, ':');
	if (!splitpath)
	{
		free(commande);
		exit(EXIT_FAILURE);
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
	free_split(splitpath);
}

int execute_single_cmd(t_env **env, char **envp, t_cmd *data)
{
	char *path;
	int pid;
	char **commande;

	if (!data->cmd[0] || !env)
		return 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (data->files && data->redirection)
			ft_do_redirections(data->files, data->redirection);
		commande = malloc(sizeof(char *) * (len_arg(&data->cmd[0]) + 1));
		if (!commande)
			exit(EXIT_FAILURE);
		execute_path(data, envp, commande);
		path = find_env(*env, "PATH");
		if (!path)
			(invalid_msg(data->cmd[0]), free(commande));
		access_and_execute(path, commande, data, envp);
		(free_split(commande), invalid_msg(data->cmd[0]), exit(EXIT_FAILURE));
	}
	else
		waitpid(pid, NULL, 0);
	return 1;
}

int ft_execute(t_exp **exp, t_env **env, t_cmd *data, char **envp)
{
	int numcmd;

	if(!data->cmd && data->files && data->redirection)
		return (ft_do_redirections(data->files, data->redirection), 1);
	if (data)
		numcmd = len_cmd(data);
	if (notpipe(data) == 1)
	{
		if (is_builtin(data->cmd[0]))
			ft_builtins(env, exp, data);
		else
			execute_single_cmd(env, envp, data);
	}
	else
	{
		execute_multi_pipe(*env, data, numcmd, envp, *exp);
	}
	return (1);
}
