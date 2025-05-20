/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:19:36 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/20 15:33:45 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	error_pipe(t_token *tokens)
{
	t_token *cur = tokens;

	while (cur)
	{
		if ((cur->type == TOKEN_PIPE && !cur->next) ||
			(cur->type == TOKEN_OR))
		{
			printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
			return 0;
		}

		cur = cur->next;
	}
	return 1;
}
int error2(t_token *cur)
{
	if (cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
		return 0;
	}
	if (cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_REDIRECT_IN)
	{
		printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
		return 0;
	}
	return 1;
}
int handle_herdoc(char *delimiter)
{
	int fd;
	char *line = NULL;
	size_t len = 0;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		printf("herdoc> ");
		getline(&line, &len, stdin);
		if (ft_strcmp(line, delimiter) == 0)
			exit(0);
		write(fd, line, ft_strlen(line));
	}
	free(line);
	close(fd);
	return 1;
}
int error(t_token *tokens)
{
    t_token *cur = tokens;

    while (cur)
    {
		if ((cur->type == TOKEN_REDIRECT_OUT && !cur->next) ||
			(cur->type == TOKEN_REDIRECT_IN && !cur->next) ||
			(cur->type == TOKEN_APPEND && !cur->next) ||
			(cur->type == TOKEN_HEREDOC && !cur->next) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_APPEND) ||
			(cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_APPEND) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_PIPE))
        {
            printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
            return 0;
        }
		else if (cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_WORD)
			handle_herdoc(cur->next->value);
		error2(cur);
        cur = cur->next;
    }
	return 1;
}
