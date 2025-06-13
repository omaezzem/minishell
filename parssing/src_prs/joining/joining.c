/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:33:09 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 23:11:40 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	split_cmd_args(char **full, char ***cmd, char ***args)
{
	int	len;

	len = 0;
	while (full && full[len])
		len++;
	if (len == 0)
	{
		*cmd = NULL;
		*args = NULL;
		return ;
	}
	*cmd = init_cmd(full);
	if (len == 1)
	{
		*args = NULL;
		return ;
	}
	*args = init_args(full, len);
}

t_cmd	*create_command_block_from_arrays(char **args,
		char **redir, char **files)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = args;
	cmd->redirection = redir;
	cmd->files = files;
	cmd->next = NULL;
	return (cmd);
}

static void	init_cmd_node(t_cmd *node)
{
	node->cmd = NULL;
	node->args = NULL;
	node->files = NULL;
	node->redirection = NULL;
	node->next = NULL;
	node->type = 0;
	node->env = NULL;
	node->value_expand = NULL;
	node->workdir = NULL;
}

static void	process_token(t_gc *gc, t_token **tokens, t_cmd *node)
{
	if ((*tokens)->type == TOKEN_WORD || (*tokens)->type == TOKEN_OPTION
		|| (*tokens)->type == TOKEN_FILE)
	{
		node->cmd = append_string_array(gc, node->cmd, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_REDIRECT_IN
		|| (*tokens)->type == TOKEN_APPEND || (*tokens)->type == TOKEN_HEREDOC)
		handle_redirection(gc, tokens, &node->redirection, &node->files);
	else
		*tokens = (*tokens)->next;
}

t_cmd	*joining2(t_gc *gc, t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*node;

	head = NULL;
	last = NULL;
	while (tokens)
	{
		node = gc_malloc(gc, sizeof(t_cmd));
		if (!node)
			return (NULL);
		init_cmd_node(node);
		while (tokens && tokens->type != TOKEN_PIPE)
			process_token(gc, &tokens, node);
		if (!head)
			head = node;
		else
			last->next = node;
		last = node;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
