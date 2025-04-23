/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:19:36 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/22 11:46:23 by omaezzem         ###   ########.fr       */
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
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_REDIRECT_IN))
        {
            printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
            return 0;
        }
        cur = cur->next;
    }
	return 1;
}
