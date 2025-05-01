/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:33:09 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/26 16:08:00 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void add_token_string(char **target, char *value)
{
	char *tmp;

	if (*target)
	{
		tmp = ft_strjoin(*target, value);
		free(*target);
		*target = tmp;
	}
	else
		*target = ft_strdup(value);

	tmp = ft_strjoin(*target, " ");
	free(*target);
	*target = tmp;
}

void handle_redirection(t_token **cur, char **redirect)
{
	char *tmp;

	if ((*cur)->next)
	{
		if (*redirect)
		{
			tmp = ft_strjoin(*redirect, (*cur)->value);
			free(*redirect);
			*redirect = tmp;
		}
		else
			*redirect = ft_strdup((*cur)->value);

		tmp = ft_strjoin(*redirect, (*cur)->next->value);
		free(*redirect);
		*redirect = tmp;

		tmp = ft_strjoin(*redirect, " ");
		free(*redirect);
		*redirect = tmp;

		*cur = (*cur)->next;
	}
}

t_cmd *create_command_block(char *command, char *option, char *redirect)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	cmd->cmd = ft_split(command, ' ');
	cmd->option = ft_split(option, ' ');
	cmd->redirection = ft_split(redirect, ' ');
	cmd->ex_status = 0;
	cmd->next = NULL;
	return cmd;
}

void print_command_blocks(t_cmd *head)
{
	int i = 1;
	t_cmd *node = head;
	while (node)
	{
		printf("\n\U0001F9F1 Command Block %d\n", i++);
		if (node->cmd)
		{
			printf("command     : ");
			int j = 0;
			while (node->cmd[j])
				printf("%s ", node->cmd[j++]);
			printf("\n");
		}
		if (node->option)
		{
			printf("option      : ");
			int j = 0;
			while (node->option[j])
				printf("%s ", node->option[j++]);
			printf("\n");
		}
		if (node->redirection)
		{
			printf("redirection : ");
			int j = 0;
			while (node->redirection[j])
				printf("%s ", node->redirection[j++]);
			printf("\n");
		}
		node = node->next;
	}
}

t_cmd *joining(t_token *tokens)
{
	t_cmd *head = NULL, *last = NULL;
	// t_env *env_list = tokens->env; // Initialize env_list here
	while (tokens)
	{
		char *cmd = NULL, *opt = NULL, *redir = NULL;
		while (tokens && tokens->type != TOKEN_PIPE)
		{
			if (tokens->type == TOKEN_WORD)
				add_token_string(&cmd, tokens->value);
			else if (tokens->type == TOKEN_OPTION)
				add_token_string(&opt, tokens->value);
			else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
			         tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
				handle_redirection(&tokens, &redir);
			tokens = tokens->next;
		}
		t_cmd *node = create_command_block(cmd, opt, redir);
		free(cmd); free(opt); free(redir);
		if (!head) head = node;
		else last->next = node;
		last = node;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	print_command_blocks(head);
	return head;
	// expand(tokens);
}
