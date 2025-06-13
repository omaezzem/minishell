/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 23:42:16 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	dyn_str_init(t_dynstr *ds)
{
	ds->data = NULL;
	ds->len = 0;
	ds->cap = 0;
}

int	dyn_str_append(t_gc *gc, t_dynstr *ds, const char *str, size_t len)
{
	char	*new_data;
	size_t	new_cap;

	if (str == NULL || len == 0)
		return (0);
	if (ds->cap < ds->len + len + 1)
	{
		new_cap = ds->cap;
		if (new_cap == 0)
			new_cap = 256;
		while (new_cap < ds->len + len + 1)
			new_cap *= 2;
		new_data = gc_malloc(gc, new_cap);
		if (new_data == NULL)
			return (-1);
		if (ds->data != NULL)
			ft_memcpy(new_data, ds->data, ds->len);
		ds->data = new_data;
		ds->cap = new_cap;
	}
	ft_memcpy(ds->data + ds->len, str, len);
	ds->len += len;
	ds->data[ds->len] = '\0';
	return (0);
}

char	*dyn_str_finalize(t_gc *gc, t_dynstr *ds)
{
	char	*new_data;

	if (!ds)
		return (NULL);
	if (ds->cap < ds->len + 1)
	{
		new_data = gc_malloc(gc, ds->len + 1);
		if (!new_data)
			return (NULL);
		ft_memcpy(new_data, ds->data, ds->len);
		ds->data = new_data;
		ds->cap = ds->len + 1;
	}
	ds->data[ds->len] = '\0';
	return (ds->data);
}

char	*handle_exit_status(t_ctx *ctx, char *result, size_t *total, char **src)
{
	char	*status_str;

	status_str = ft_itoa(ctx->exit_status);
	if (!status_str)
		return (result);
	result = append_to1_result1(ctx->gc, result, status_str, total);
	free(status_str);
	(*src)++;
	return (result);
}

char	*expand_dollar_variable(t_ctx *ctx, char *result,
		char **src, size_t *total)
{
	char	varname[256];

	if (**src == '?')
	{
		result = handle_exit_status(ctx, result, total, src);
		return (result);
	}
	if (!extract_varname(src, varname))
	{
		result = append_to1_result1(ctx->gc, result, "$", total);
		return (result);
	}
	return (append_expanded_var(ctx, result, varname, total));
}
