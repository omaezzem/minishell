/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:21:14 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/12 11:23:16 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	**append_string_array(t_gc *gc, char **arr, char *str)
{
	int		len;
	char	**new_arr;
	int		i;

	len = 0;
	while (arr && arr[len])
		len++;
	new_arr = gc_malloc(gc, sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[len] = gc_strdup(gc, str);
	new_arr[len + 1] = NULL;
	return (new_arr);
}

void	handle_redirection(t_gc *gc, t_token **cur,
		char ***redir, char ***files)
{
	t_token	*str;

	str = *cur;
	*redir = append_string_array(gc, *redir, str->value);
	str = str->next;
	while (str && str->type == TOKEN_SPACE)
		str = str->next;
	if (str && str->type == TOKEN_FILE)
	{
		*files = append_string_array(gc, *files, str->value);
		str = str->next;
	}
	else
		*files = append_string_array(gc, *files, gc_strdup(gc, ""));
	*cur = str;
}

char	**init_cmd(char **full)
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * 2);
	if (!cmd)
		return (NULL);
	cmd[0] = ft_strdup(full[0]);
	cmd[1] = NULL;
	return (cmd);
}

char	**init_args(char **full, int len)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * len);
	if (!args)
		return (NULL);
	i = 1;
	while (i < len)
	{
		args[i - 1] = ft_strdup(full[i]);
		i++;
	}
	args[len - 1] = NULL;
	return (args);
}
