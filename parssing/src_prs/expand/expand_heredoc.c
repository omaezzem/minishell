/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:39:51 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 21:02:55 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*handle_dollar_sequence(t_ctx *ctx, char *result,
		char **src_ptr, size_t *total)
{
	char	*src;
	int		escape_count;
	int		expand;

	src = *src_ptr;
	count_dollars(&src, &escape_count, &expand);
	while (escape_count-- > 0)
		result = append_to1_result1(ctx->gc, result, "$", total);
	if (expand)
		result = expand_dollar_variable(ctx, result, &src, total);
	*src_ptr = src;
	return (result);
}

void	count_dollars(char **src, int *escape_count, int *expand)
{
	int	count;

	count = 0;
	while (**src == '$')
	{
		count++;
		(*src)++;
	}
	*escape_count = count / 2;
	*expand = count % 2;
}

int	extract_varname(char **src, char *varname)
{
	int	i;

	i = 0;
	if (isalpha(**src) || isdigit(**src) || **src == '_')
	{
		while ((isalnum(**src) || **src == '_') && i < 255)
			varname[i++] = *((*src)++);
		varname[i] = '\0';
		return (i > 0);
	}
	return (0);
}

char	*append_expanded_var(t_ctx *ctx, char *result,
		char *varname, size_t *total)
{
	char	*val;

	val = find_env(ctx->env, varname);
	if (!val)
		val = "";
	return (append_to1_result1(ctx->gc, result, val, total));
}

char	*expand_heredoc(t_ctx *ctx, t_token *token)
{
	char	*result;
	size_t	total;
	char	*src;
	char	tmp[2];

	result = NULL;
	total = 0;
	while (token)
	{
		src = token->value;
		while (*src)
		{
			if (*src == '$')
				result = handle_dollar_sequence(ctx, result, &src, &total);
			else
			{
				tmp[0] = *src;
				tmp[1] = '\0';
				result = append_to1_result1(ctx->gc, result, tmp, &total);
				src++;
			}
		}
		token = token->next;
	}
	return (result);
}
