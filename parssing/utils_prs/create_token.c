/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:22:10 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/17 20:36:46 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*create_token(t_gc *gc, char *value, t_type type)
{
	t_token	*token;

	token = gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = gc_strdup(gc, value);
	token->type = type;
	token->next = NULL;
	return (token);
}
