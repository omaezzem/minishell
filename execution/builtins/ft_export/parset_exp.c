/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parset_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:32:24 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:55:26 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*args_zero(char *args)
{
	int		i;
	char	*zero;
	int		len;

	if (!args)
		return (NULL);
	if (!ft_isalpha(args[0]))
		return (minishell_invalid(args), NULL);
	len = len_at_first_equal(args);
	zero = malloc(len + 1);
	if (!zero)
		return (NULL);
	i = 0;
	while (args[i])
	{
		if (args[i] == '=')
			break ;
		zero[i] = args[i];
		i++;
	}
	zero[i] = '\0';
	return (zero);
}

char	*args_one(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '=')
		{
			if (args[i + 1] != '\0')
				return (&args[i + 1]);
		}
		i++;
	}
	return (NULL);
}
