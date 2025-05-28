/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:43:20 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/27 23:12:39 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_pwd(t_env **env_head)
{
	t_env	*new_node;
	char	cwd[PATH_MAX];

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd");
	new_node->var = ft_strdup("PWD");
	new_node->val = ft_strdup(cwd);
	new_node->next = NULL;
	ft_lstadd_back_env(env_head, new_node);
}

void	add_path(t_env **env_head)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->var = ft_strdup("PATH");
	new_node->val = ft_strdup(PATH_ENV);
	new_node->next = NULL;
	ft_lstadd_back_env(env_head, new_node);
}

void	add_usr_bin_env(t_env **env_head)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->var = ft_strdup("_");
	new_node->val = ft_strdup("/usr/bin/env");
	new_node->next = NULL;
	ft_lstadd_back_env(env_head, new_node);
}

void	add_shlvl(t_env **env_head)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->var = ft_strdup("SHLVL");
	new_node->val = ft_strdup("1");
	new_node->next = NULL;
	ft_lstadd_back_env(env_head, new_node);
}

int	builtin_env(t_env **env, char **args)
{
	t_env	*current;

	if (args && args[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (0);
	}

    current = *env;
	if (current == NULL)
		return 0;
	while (current != NULL)
	{
		if (current->var && current->val)
		{
			ft_putstr_fd(current->var, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(current->val, 1);
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
	return (1);
}
