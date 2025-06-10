/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:43:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/08 18:21:22 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	minishell_invalid(char *invalid_str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(invalid_str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

char	*check_var(char *var)
{
	int i;
	int	lenvar;
	char *parsvar;

	if (!var)
		return NULL;
	lenvar = len_alnum_var(var);
	parsvar = malloc(sizeof(char) * lenvar + 1);
	if (!parsvar)
		return (NULL);
	if (ft_isnum(var[0]))
		return (minishell_invalid(var), free(var), free(parsvar), NULL);
	i = -1;
	while (var[++i])
	{
		if (var[i] == '+' &&  var[i+1] != '\0')
			return (minishell_invalid(var), free(var), free(parsvar), NULL);
		if (!ft_isalnum(var[i]))
		{
			if (var[i-1])
			{
				if (ft_isalnum(var[i-1]) && var[i] == '+' &&  var[i+1] == '\0')
					break ;
			}
			return (minishell_invalid(var), free(var) ,free(parsvar), NULL);
		}
		parsvar[i] = var[i];
	}
	free(var);
	parsvar[i] = '\0';
	return parsvar;
}
