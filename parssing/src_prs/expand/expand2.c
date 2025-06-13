/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:42:26 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 21:09:32 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*expand_value(t_ctx *ctx, char *value, t_cmd *cmd)
{
	t_dynstr	ds;
	const char	*src;
	int			s;
	int			d;
	char		tmp[2];

	s = 0;
	d = 0;
	src = value;
	ctx->s = &s;
	ctx->d = &d;
	dyn_str_init(&ds);
	while (*src)
	{
		if ((*src == '\'' && !ctx->d) || (*src == '"' && !ctx->s))
			handle_quotes(ctx, *src++, &ds);
		else if (*src == '$' && !s)
			handle_dollar(ctx, &src, &ds, cmd);
		else
		{
			tmp[0] = *src++;
			dyn_str_append(ctx->gc, &ds, tmp, 1);
		}
	}
	return (dyn_str_finalize(ctx->gc, &ds));
}

t_token	*add_expanded_tokens(t_gc *gc, char *value, t_token **cur)
{
	t_token	*new_tks;
	t_token	*next;

	new_tks = tokenize1(gc, value);
	while (new_tks)
	{
		next = new_tks->next;
		new_tks->next = NULL;
		*cur = new_tks;
		cur = &new_tks->next;
		new_tks = next;
	}
	return (*cur);
}

void	handle_expansion(t_ctx *ctx, t_token **cur, t_token *tk, t_cmd *cmd)
{
	char	*exp;
	t_token	*tmp;
	t_token	*last;

	exp = expand_value(ctx, tk->value, cmd);
	tmp = tokenize1(ctx->gc, exp);
	last = *cur;
	while (tmp)
	{
		if (last)
			last->next = tmp;
		last = tmp;
		tmp = tmp->next;
	}
	*cur = last;
}

t_token	*expand(t_ctx *ctx, t_token *tk, t_cmd *cmd)
{
	t_token	head;
	t_token	*cur;
	t_token	*next;

	head.next = NULL;
	cur = &head;
	while (tk)
	{
		next = tk->next;
		tk->next = NULL;
		if (tk->type == TOKEN_WORD && ft_strchr(tk->value, '$'))
		{
			handle_expansion(ctx, &cur, tk, cmd);
		}
		else
		{
			cur->next = tk;
			cur = cur->next;
		}
		tk = next;
	}
	return (head.next);
}

char	*append_to1_result1(t_gc *gc, char *result, char *str, size_t *total)
{
	size_t	len;
	char	*new_result;

	len = ft_strlen(str);
	new_result = gc_malloc(gc, *total + len + 1);
	if (!new_result)
		return (NULL);
	if (result)
		ft_memcpy(new_result, result, *total);
	ft_memcpy(new_result + *total, str, len);
	*total += len;
	new_result[*total] = '\0';
	free(result);
	return (new_result);
}
