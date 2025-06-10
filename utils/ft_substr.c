/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:18:31 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/16 17:00:15 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_substr(char *s, int start, int len)
{
	int		i;
	char	*sub;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	sub = (char *)malloc (sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
char *gc_substr(t_gc *gc, char *s, int start, int len)
{
	int i;
	char *sub;

	if (!s)
		return NULL;
	if (start >= ft_strlen(s))
		return gc_strdup(gc, "");
	if (len > (int)ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	sub = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
	if (!sub)
		return NULL;
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return sub;
}
