/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/30 16:44:21 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int prev_status;

int expand_status(char **newp, int space_left)
{
	int n, i = 0;
	char status_buffer[10];
	char *new = *newp;
	n = snprintf(status_buffer, 10, "%d", prev_status);
	if (n > space_left)
		return ENOROOM;
	while (status_buffer[i] != '\0')
		*new++ = status_buffer[i++];
	*newp = new;
	return n;
}

int expand_argc(char **newp, int space_left)
{
	char argc_buffer[10], *new = *newp;
	int n, i = 0;
	if (cmdline_argc < cmdline_shift)
	{
		n = snprintf(argc_buffer, 10, "0");
	}
	else
	{
		n = snprintf(argc_buffer, 10, "%d", cmdline_argc - cmdline_shift);
	}
	if (n > space_left)
		return ENOROOM;
	while (argc_buffer[i] != '\0')
		*new++ = argc_buffer[i++];
	*newp = new;
	return n;
}

int expand_env(char **oldp, char **newp, int brace_flag)
{
	int i = 0, n = 0;
	int space_left = 0;
	char env_buffer[MAX_ENV], *env_val;
	char *old = *oldp;
	char *new = *newp;

	if (!brace_flag)
	{
		// Unbraced variable: parse valid var name chars (letters, digits, underscores)
		while (isalnum(*old) || *old == '_')
			env_buffer[i++] = *old++;
	}
	else
	{
		// Braced variable: skip {, parse until }
		old++; // skip {
		while (*old != '}')
		{
			if (*old == '\0')
				return ENOBRACE; // unmatched brace
			env_buffer[i++] = *old++;
		}
		old++; // skip }
	}

	env_buffer[i] = '\0';

	// Fetch environment variable
	env_val = getenv(env_buffer);
	space_left = ft_strlen(env_val);
	if (!space_left)
	{
		// No such variable, return empty string
		env_val = "";
		space_left = 1;
	}
	// Copy env_val to new string if exists
	if (env_val != NULL)
	{
		n = ft_strlen(env_val);
		if (n > space_left)
			return ENOROOM;

		while (*env_val != '\0')
			*new++ = *env_val++;
	}

	*oldp = old - 1; // go back to last valid char
	*newp = new;
	return n;
}

int expand_argv(char **oldp, char **newp, int space_left)
{
	char arg_idx[4], *arg;
	char *old = *oldp;
	char *new = *newp;
	int argn, n = 0, i = 0;
	while (isdigit(*old))
		arg_idx[i++] = *old++;
	arg_idx[i] = '\0';
	argn = atoi(arg_idx);
	if ((argn + cmdline_shift) < (cmdline_argc))
	{
		n = ft_strlen(cmdline_argv[argn + cmdline_shift]);
		if (n > space_left)
			return ENOROOM;
		arg = cmdline_argv[argn + cmdline_shift];
		while (*arg != '\0')
			*new++ = *arg++;
	}
	*oldp = (old - 1);
	*newp = new;
	return n;
}

int expand_pid(char **newp, int space_left)
{
	int i, n;
	pid_t pid;
	char pid_buffer[10];
	char *new = *newp;
	pid = getpid();
	snprintf(pid_buffer, 10, "%d", pid);
	if ((n = ft_strlen(pid_buffer)) > space_left)
		return ENOROOM;
	i = 0;
	while (pid_buffer[i] != '\0')
		*new++ = pid_buffer[i++];
	*newp = new;
	return n;
}

int expand(t_token *token)
{
	char *old = NULL;
	int oldlen = 0;
	int newsize = 0;
	char *new = NULL;
	t_env *env = malloc(sizeof(t_env));
	t_token *tmp_token = token;
	char *newp;
	int rv;

	// while (*old && *old != '$') {
	// 	*old = *old + 1;
	// }
	while (tmp_token)
	{
		old = tmp_token->value;
		env->var = old;
		oldlen = ft_strlen(old);
		newsize = oldlen * 2;
		new = malloc(newsize * 2 + 1);
		newp = new;
		if (!new)
			return 0; // Memory allocation failed
		while (*old != '\0')
		{
			if (*old == '"')
				old++;
			if (*old == '$')
			{
				old++;
				if (*old == '\0')
					return 0; // No variable name after $
				if (*old == '$') {
					old++;
					rv = expand_pid(&newp, newsize - (newp - new));
					if (rv < 0)
						return 0;
				}
				else if (*old == '?') {
					old++;
					rv = expand_status(&newp, newsize - (newp - new));
					if (rv < 0)
						return 0;
				}
				else if (*old == '#') {
					old++;
					rv = expand_argc(&newp, newsize - (newp - new));
					if (rv < 0)
						return 0;
				}
				else if (isdigit(*old)) {
					rv = expand_argv(&old, &newp, newsize - (newp - new));
					if (rv < 0)
						return 0;
					old++; // move past the digit(s) already handled
				}
				else
				{
					int brace_flag = (*old == '{');
					printf("%d\n", newsize);
					rv = expand_env(&old, &newp, brace_flag);
					if (rv < 0)
						return 0;
					old++; // advance past the variable name / closing brace
				}
			}
			else
			{
				*newp++ = *old++;
			}
		}
	tmp_token = tmp_token->next;
	}


	*newp = '\0';

	free(token->value);
	env->val = new;
	printf("Expanded value: %s\n", env->val); // Print the expanded value
	printf("Expanded var: %s\n", env->var); // Print the expanded value
	return 1;
}
