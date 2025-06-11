/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:05:28 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 23:13:26 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void ft_close_wait(int numcmd, int (*pipes)[2], int *pids, t_cmd *data)
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
    {
        waitpid(pids[i], &status, 0);
        if (i == numcmd - 1)
            update_exit_status(status, data);
    }
    signal(SIGINT, SIG_IGN);
    if (WTERMSIG(status) == SIGINT)
        write(2, "\n", 1);
    if (WTERMSIG(status) == SIGQUIT)
        write(2, "Quit: 3\n", 8);
}

static int setup_resources(t_ctx *ctx)
{
	if (ctx->numcmd > 1)
	{
		ctx->pipes = malloc((ctx->numcmd - 1) * sizeof(*ctx->pipes));
		if (!ctx->pipes)
			return (perror("malloc"), exit_failure(ctx->data), 0);
	}
	ctx->pids = malloc(ctx->numcmd * sizeof(int));
	if (!ctx->pids)
		return (perror("malloc"), free_pipes(ctx->pipes, ctx->data, ctx->pids, ctx->numcmd), 0);
	create_pipes(ctx->numcmd, ctx->pipes, ctx->pids, ctx->data);
	return (1);
}

int		ft_execute_pipes(t_ctx *ctx)
{
	t_cmd	*curr;
	int		i;

	curr = ctx->data;
	i = -1;
	while (++i < ctx->numcmd && curr)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
			return (perror("fork"), 0);
		if (ctx->pids[i] == 0)
			(do_child_one(i, ctx, curr),
				do_child_two(curr, i, ctx));
		curr = curr->next;
	}
	return (1);
}

int	execute_multi_pipe(t_cmd *data, char **envp, int fdher, t_env *env, t_exp *exp)
{
	t_ctx	ctx;
	int		ret;

	ctx.env = env;
	ctx.exp = exp;
	ctx.numcmd = len_cmd(data);
	ctx.envp = envp;
	ctx.data = data;
	ctx.fdher = fdher;
	ctx.pids = NULL;
	ctx.pipes = NULL;
	if (!setup_resources(&ctx))
		return (0);
	ret = ft_execute_pipes(&ctx);
	ft_close_wait(ctx.numcmd, ctx.pipes, ctx.pids, data);
	if (ctx.numcmd > 1)
		free(ctx.pipes);
	free(ctx.pids);
	return (ret);
}

int ft_execute(t_exp **exp, t_env **env, t_cmd **data, char **envp, t_heredoc *heredoc)
{
	int fdher;
	int numcmd;

	numcmd = 0;
	if (data)
		numcmd = len_cmd(*data);
	fdher = heredoc->fd;
	int std_in = dup(0);
	if(!(*data)->cmd && (*data)->files && (*data)->redirection)
		ft_do_redirections((*data)->files, (*data)->redirection, fdher, *data);
	if (notpipe(*data) == 1)
	{
		if (is_builtin((*data)->cmd[0]))
		{
			ft_builtins(env, exp, *data, fdher);
		}
		else
			execute_single_cmd(env, envp, *data, heredoc);
	}
	else
		execute_multi_pipe(*data, envp, fdher, *env, *exp);
	return ((dup2(std_in, 0), close(std_in)), 1);
}
