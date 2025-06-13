/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mysplit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:05:02 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 21:20:55 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	ft_strlcpy(char *dst, char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	if (dstsize == 0)
		return (len);
	i = 0;
	while (src[i] && i < (dstsize - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

static	int	count_str(char *str, char c)
{
	int	i;
	int	len;
	int	isword;

	i = 0;
	len = 0;
	isword = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			isword = 0;
		}
		else if (isword == 0)
		{
			isword = 1;
			len++;
		}
		i++;
	}
	return (len);
}

static size_t	str_length(char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	**freememory(char **s, int i)
{
	int	index;

	index = 0;
	while (index < i)
	{
		free(s[index]);
		index++;
	}
	free(s);
	return (NULL);
}

char	**mysplit(char *s, char c)
{
	size_t	len;
	size_t	index;
	char	**split;

	if (!s)
		return (NULL);
	len = count_str((char *)s, c);
	index = 0;
	split = (char **) malloc((sizeof(char *)) * (len + 1));
	if (!split)
		return (NULL);
	while (index < len)
	{
		while (*s == c)
			s++;
		split[index] = (char *)malloc((sizeof(char) * (str_length(s, c) + 1)));
		if (!split[index])
			return (freememory(split, index));
		ft_strlcpy(split[index], s, str_length(s, c) + 1);
		s = s + str_length(s, c);
		index++;
	}
	split[index] = NULL;
	return (split);
}
