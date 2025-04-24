/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:22:10 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/24 09:04:51 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token *create_token(char *value, t_type type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->value = strdup(value);
	token->type = type;
	token->next = NULL;
	return token;
}
