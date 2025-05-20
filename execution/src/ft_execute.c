/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/20 19:11:27 by omaezzem         ###   ########.fr       */
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
int ft_builtins(t_env *env, t_exp *exp, t_cmd *data)
{
    if (data->files && data->redirection && notpipe(data))
        ft_do_redirections(data->files, data->redirection);
    if (ft_strcmp(data->cmd[0], "echo") == 0)
        return(ft_echo(&data->cmd[0]), 1);
    else if (ft_strcmp(data->cmd[0], "cd") == 0)
        return (ft_cd(env, data->cmd), 1);
    else if (ft_strcmp(data->cmd[0], "pwd") == 0)
        return (ft_pwd(), 1);
    else if (ft_strcmp(data->cmd[0], "export") == 0)
        return (ft_export(exp, env, data->cmd), 1);
    else if (ft_strcmp(data->cmd[0], "env") == 0)
        return (builtin_env(&env, data->cmd), 1);
    else if (ft_strcmp(data->cmd[0], "exit") == 0)
        return (ft_exit(data, data->cmd), 1);
    else if (ft_strcmp(data->cmd[0], "unset") == 0)
        return (ft_unset(&exp, &env, data->cmd), 1);
    else
        return (0);
}
int is_builtin(char **args)
{
    if (!ft_strcmp(args[0], "echo")
        || !ft_strcmp(args[0], "export")
        || !ft_strcmp(args[0], "env")
        || !ft_strcmp(args[0], "unset")
        || !ft_strcmp(args[0], "exit")
        || !ft_strcmp(args[0], "pwd")
        || !ft_strcmp(args[0], "cd"))
        return 1;
    return 0;
}

int index_last_file(char **files)
{
    int i = 0;

    while (files[i + 1] != NULL)
    {
        i++;
    }
    return i;
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
    int i_fl;
    int i;
    int j;

    if (files)
        i_fl = index_last_file(files);
    if (files && redirections)
    {
        if (ft_len_redirections(redirections) == 1)
            to_single_redirection(files, redirections);
        else
        {
            i = 0;
            j = 0;
            while (files[j] && j <= i_fl)
            {
                if (ft_strcmp(redirections[i], ">") == 0
                    || ft_strcmp(redirections[i], ">>") == 0)
                {
                        ft_output_append(files, redirections, i);
                }
                else if (ft_strcmp(redirections[i], "<") == 0
                    || ft_strcmp(redirections[i], "<<") == 0)
                {
                        ft_inp_heredoc(files, redirections, i);
                }
                i++;
                j++;
            }
            if (files[j] != NULL)
            {
                if (ft_strcmp(redirections[i], ">") == 0
                    || ft_strcmp(redirections[i], ">>") == 0)
                {
                    fd = ft_output_append(files, redirections, i);
                    if (dup2(fd, STDOUT_FILENO) == -1)
                        return (close(fd), perror("minishell"), FAILURE);
                }
                else if (ft_strcmp(redirections[i], "<") == 0
                    || ft_strcmp(redirections[i], "<<") == 0)
                {
                    fd = ft_inp_heredoc(files, redirections, i);
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
    while (i < numcmd)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            // leks pipes
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0)
        {
            if (curr->files && curr->redirection)
            {
                ft_do_redirections(curr->files, curr->redirection);
            }
            if (i > 0)
                dup2(pipes[i-1][0], STDIN_FILENO);
            if (i < numcmd - 1)
                dup2(pipes[i][1], STDOUT_FILENO);
            j = 0;
            while (j < numcmd - 1)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
                j++;
            }
            if (is_builtin(curr->cmd))
            {
                ft_builtins(env, exp, curr);
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
                if (access(curr->cmd[0], X_OK | F_OK) == 0)
                {
                    commande[0] = ft_strdup(curr->cmd[i]);
                    commande[1] = NULL;
                    execve(curr->cmd[0], commande, envp);
                }
                j = 0;
                while (curr->cmd[j])
                {
                    commande[j] = ft_strdup(curr->cmd[j]);
                    j++;
                }
                commande[j] = NULL;
                path = find_env(env, "PATH");
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
                        perror("execve");
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
                    ft_putstr_fd(": command not found\n", 2);
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

int execute_single_cmd(t_env *env, char **envp, t_cmd *data)
{
    char *path;
    char **splitpath;
    char *joinslash;
    char *cmd;
    int i;
    int pid;
    char **commande;
    int lenargs;


    if (!data->cmd[0] || !env)
        return 0;

    lenargs = len_arg(&data->cmd[0]);
    pid = fork();
    if (pid == 0)
    {
        ft_do_redirections(data->files, data->redirection);
        i = 0;
        commande = malloc(sizeof(char *) * (lenargs + 1));
        if (!commande)
            exit(EXIT_FAILURE);
        if (access(data->cmd[0], X_OK | F_OK) == 0)
        {
            commande[0] = ft_strdup(data->cmd[i]);
            commande[1] = NULL;
            execve(data->cmd[0], commande, envp);
        }
        path = find_env(env, "PATH");
        if (!path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(data->cmd[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            free(commande);
            exit(EXIT_FAILURE);
        }
        splitpath = mysplit(path, ':');
        if (!splitpath)
        {
            free(commande);
            exit(EXIT_FAILURE);
        }
        i = 0;
        while (data->cmd[i])
        {
            commande[i] = ft_strdup(data->cmd[i]);
            i++;
        }
        commande[i] = NULL;
        i = 0;
        while (splitpath[i])
        {
            joinslash = ft_strjoin(splitpath[i], "/");
            cmd = ft_strjoin(joinslash, data->cmd[0]);
            free(joinslash);
            if (access(cmd, X_OK | F_OK) == 0)
                execve(cmd, commande, envp);
            i++;
        }
        free_split(commande);
        free_split(splitpath);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(data->cmd[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(EXIT_FAILURE);
    }
    else
        waitpid(pid, NULL, 0);
    return 1;
}

int ft_execute(t_exp *exp, t_env *env, t_cmd *data, char **envp)
{
    int numcmd;
    char *shlvl;
    int  n_shlvl;
    char *itoa_shlvl;

    if (data)
        numcmd = len_cmd(data);
    if (data->cmd[0])
    {
        if (notpipe(data) == 1)
        {
            if (!ft_strcmp(data->cmd[0], "./minishell"))
            {
                shlvl = find_env(env, "SHLVL");
                printf("%s\n", shlvl);
                n_shlvl = mini_atoi(shlvl);
                n_shlvl++;
                printf("%d\n", n_shlvl);
                itoa_shlvl = ft_itoa(n_shlvl);
                printf("%s\n", itoa_shlvl);
                update_val_env(env, "SHLVL", itoa_shlvl);
                update_val_exp(exp, "SHLVL", itoa_shlvl);
            }
            else if (data->cmd[0][0] == '.' && data->cmd[0][1] == '/' && ft_strcmp(data->cmd[0], "./minishell"))
                return (check_dir_file(data->cmd), 0);
            else if (is_builtin(data->cmd))  
                ft_builtins(env, exp, data);
            else
                execute_single_cmd(env, envp, data);
        }
        else
        {
            execute_multi_pipe(env, data, numcmd, envp, exp);
        }
    }
	return (1);
}
