/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:57:53 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 20:02:54 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	add_word1(t_gc *gc, char *input, t_token **head, int i)
{
	int		start;
	char	quote;
	char	*tmp;
	t_token	*new_token;

	start = i;
	while (input[i] && !ft_issspace(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (-1);
			i++;
		}
		else
			i++;
	}
	tmp = gc_substr(gc, input, start, i - start);
	new_token = create_token(gc, tmp, TOKEN_WORD);
	append_token(head, new_token);
	return (i);
}

t_token	*tokenize(t_gc *gc, char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i] && ft_issspace(input[i]))
		i++;
	while (input[i])
	{
		i = tokenize_next_token(gc, input, &head, i);
		if (i < 0)
			return (NULL);
	}
	return (head);
}

t_token	*tokenize2(t_gc *gc, char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i] && ft_issspace(input[i]))
		i++;
	while (input[i])
	{
		if (ft_issspace(input[i]))
			i++;
		else if (redir_pipe(input[i]))
			i = handle_redir(gc, input, &head, i);
		else
			i = add_word(gc, input, &head, i);
		if (i < 0)
			return (NULL);
	}
	return (head);
}

int	skip_spaces(t_gc *gc, char *input, int i, t_token **head)
{
	int		start;
	char	*tmp;

	start = i;
	while (ft_issspace(input[i]))
		i++;
	tmp = gc_substr(gc, input, start, i - start);
	if (!tmp)
		return (-1);
	append_token(head, create_token(gc, tmp, TOKEN_SPACE));
	return (i);
}

t_token	*tokenize1(t_gc *gc, char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_issspace(input[i]))
			i = skip_spaces(gc, input, i, &head);
		else
			i = add_word1(gc, input, &head, i);
		if (i < 0)
			return (NULL);
	}
	return (head);
}
