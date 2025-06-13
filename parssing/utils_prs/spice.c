/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spice.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:06:32 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 20:12:16 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_issspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	redir_pipe(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '(' || c == ')');
}

int	gc_issspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

int	is_empty_quotes(char *str, int i)
{
	return ((str[i] == '"' && str[i + 1] == '"')
		|| (str[i] == '\'' && str[i + 1] == '\''));
}

int	skip_quotes(const char *s, int i)
{
	char	quote;

	quote = s[i++];
	while (s[i] && s[i] != quote)
	{
		if (s[i] == '\\' && quote == '\"' && s[i + 1])
			i += 2;
		else
			i++;
	}
	if (s[i] == quote)
		return (i + 1);
	else
		return (i);
}
