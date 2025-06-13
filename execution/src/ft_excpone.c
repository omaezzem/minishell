/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excpone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:53:16 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 19:43:42 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_pipes(int (*pipes)[2], t_cmd *data, int *pids, int numcmd)
{
	if (numcmd > 1 && pipes != NULL)
		free(pipes);
	if (pids != NULL)
		free(pids);
	exit_failure(data);
}

int	create_pipes(int numcmd, int (*pipes)[2], int *pids, t_cmd *data)
{
	int	i;

	if (numcmd <= 1)
		return (1);
	i = -1;
	while (++i < (numcmd - 1))
	{
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), free_pipes(pipes, data, pids, numcmd), 0);
	}
	return (1);
}

int	do_child_one(int i, t_ctx *ctx, t_cmd *curr)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!curr->cmd || !curr->cmd[0])
		exit_one_handred_twenty_seven(ctx->data);
	if (!dup_and_close_pipes(ctx->pipes, i, ctx->numcmd))
		exit_failure(ctx->data);
	if (curr->files && curr->redirection)
		ft_do_redirections(curr->files, curr->redirection,
			ctx->fdher, ctx->data);
	return (1);
}

int	do_child_two(t_cmd *curr, int i, t_ctx *ctx)
{
	if (curr->cmd && is_builtin(curr->cmd[0]))
	{
		ft_builtins(&ctx->env, &ctx->exp, curr, ctx->fdher);
		curr = curr->next;
		i++;
		exit_success(ctx->data);
	}
	else
		(ft_execve(curr, &ctx->env, ctx, i),
			invalid_msg(curr->cmd, ctx->data));
	return (1);
}

int	dup_and_close_pipes(int (*pipes)[2], int i, int numcmd)
{
	int	j;

	if (i > 0 && pipes[i - 1][0] != -1)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
		close(pipes[i - 1][0]);
		pipes[i - 1][0] = -1;
	}
	if (i < numcmd - 1 && pipes[i][1] != -1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
		close(pipes[i][1]);
	}
	j = -1;
	while (++j < numcmd - 1)
	{
		if (pipes[j][0] != -1)
			close(pipes[j][0]);
		if (pipes[j][1] != -1)
			close(pipes[j][1]);
	}
	return (1);
}
