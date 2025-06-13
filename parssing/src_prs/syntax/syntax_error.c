/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:19:36 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 22:15:33 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	quote_handler(char *str, size_t *i, size_t *j, char *res)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		res[(*j)++] = str[*i];
		(*i)++;
	}
	if (str[*i] == quote)
		(*i)++;
}

char	*strip2_quotes(t_gc *gc, char *str)
{
	size_t	i;
	size_t	j;
	char	*res;

	i = 0;
	j = 0;
	res = gc_malloc(gc, ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			quote_handler(str, &i, &j, res);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

void	remove_quotes2_token(t_gc *gc, t_token *token)
{
	char	*tmp;

	while (token)
	{
		if (token->value)
		{
			tmp = strip2_quotes(gc, token->value);
			token->value = tmp;
		}
		token = token->next;
	}
}
