/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collectore.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:36:08 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 20:36:08 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	gc_init(t_gc *gc)
{
	gc->list = NULL;
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = gc->list;
	gc->list = node;
	return (ptr);
}

void	gc_free_all(t_gc *gc)
{
	t_gc_node	*tmp;

	while (gc->list)
	{
		tmp = gc->list;
		gc->list = gc->list->next;
		if (tmp->ptr)
			free(tmp->ptr);
		free(tmp);
	}
	gc->list = NULL;
}
