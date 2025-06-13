/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:43:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/12 10:55:38 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	minishell_invalid(char *invalid_str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(invalid_str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	validate_var(char *var, char *parsvar)
{
	int	i;

	i = -1;
	while (var[++i])
	{
		if (var[i] == '+' && var[i + 1] != '\0')
			return ((minishell_invalid(var), free(var), free(parsvar)), 0);
		if (!ft_isalnum(var[i]))
		{
			if (var[i - 1] && ft_isalnum(var[i - 1])
				&& var[i] == '+' && var[i + 1] == '\0')
				break ;
			return ((minishell_invalid(var), free(var), free(parsvar)), 0);
		}
		parsvar[i] = var[i];
	}
	parsvar[i] = '\0';
	return (i);
}

char	*check_var(char *var)
{
	int		lenvar;
	int		final_len;
	char	*parsvar;

	if (!var)
		return (NULL);
	lenvar = len_alnum_var(var);
	parsvar = malloc(sizeof(char) * lenvar + 1);
	if (!parsvar)
		return (NULL);
	if (ft_isnum(var[0]))
		return (minishell_invalid(var), NULL);
	final_len = validate_var(var, parsvar);
	if (!final_len)
		return (NULL);
	free(var);
	return (parsvar);
}
