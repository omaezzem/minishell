/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:58:43 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 12:16:54 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_escaped_quote(char *str, int i)
{
	return (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '"'
			|| str[i + 1] == '\\'));
}

int	handle_quoted_content(char *str, int i, char *res, int *j)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		res[(*j)++] = str[i++];
	if (str[i] == quote)
		i++;
	return (i);
}

char	*strip_quotes(t_gc *gc, char *str)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	res = gc_malloc(gc, ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_empty_quotes(str, i))
		{
			res[j++] = ' ';
			i += 2;
		}
		else if (is_escaped_quote(str, i))
		{
			res[j++] = str[i + 1];
			i += 2;
		}
		else if (str[i] == '\'' || str[i] == '"')
			i = handle_quoted_content(str, i, res, (int *)&j);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

void	remove_quotes_array(t_gc *gc, char **arr)
{
	int		i;
	char	*stripped;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		stripped = strip_quotes(gc, arr[i]);
		arr[i] = stripped;
		i++;
	}
}

void	remove_quotes_cmd(t_gc *gc, t_cmd *cmd)
{
	while (cmd)
	{
		remove_quotes_array(gc, cmd->cmd);
		remove_quotes_array(gc, cmd->args);
		remove_quotes_array(gc, cmd->files);
		remove_quotes_array(gc, cmd->redirection);
		cmd = cmd->next;
	}
}
