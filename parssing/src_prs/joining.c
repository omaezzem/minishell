/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:33:09 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/22 11:51:57 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void add_token_string(char **target, char *value)
{
	char *tmp;

	if (*target)
	{
		tmp = ft_strjoin(*target, value);
		free(*target);
		*target = tmp;
	}
	else
		*target = ft_strdup(value);

	tmp = ft_strjoin(*target, " ");
	free(*target);
	*target = tmp;
}

void handle_redirection(t_token **cur, char **redirect)
{
	char *tmp;

	if ((*cur)->next)
	{
		if (*redirect)
		{
			tmp = ft_strjoin(*redirect, (*cur)->value);
			free(*redirect);
			*redirect = tmp;
		}
		else
			*redirect = ft_strdup((*cur)->value);

		tmp = ft_strjoin(*redirect, (*cur)->next->value);
		free(*redirect);
		*redirect = tmp;

		tmp = ft_strjoin(*redirect, " ");
		free(*redirect);
		*redirect = tmp;

		*cur = (*cur)->next;
	}
}

t_cmd *create_command_block(char *command, char *option, char *redirect)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	cmd->cmd = ft_split(command, ' ');
	cmd->option = ft_split(option, ' ');
	cmd->redirection = ft_split(redirect, ' ');
	cmd->ex_status = 0;
	cmd->next = NULL;
	return cmd;
}

void print_command_blocks(t_cmd *head)
{
	int i = 1;
	t_cmd *node = head;
	while (node)
	{
		printf("\n\U0001F9F1 Command Block %d\n", i++);
		if (node->cmd)
		{
			printf("command     : ");
			int j = 0;
			while (node->cmd[j])
				printf("%s ", node->cmd[j++]);
			printf("\n");
		}
		if (node->option)
		{
			printf("option      : ");
			int j = 0;
			while (node->option[j])
				printf("%s ", node->option[j++]);
			printf("\n");
		}
		if (node->redirection)
		{
			printf("redirection : ");
			int j = 0;
			while (node->redirection[j])
				printf("%s ", node->redirection[j++]);
			printf("\n");
		}
		node = node->next;
	}
}/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/22 11:48:22 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

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

char* gitenv(const char* name) {
	return getenv(name);
}

int expand_env(char **oldp, char **newp, int space_left, int brace_flag)
{
	int i = 0, n = 0;
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

	// Copy env_val to new string if exists
	if (env_val != NULL)
	{
		n = strlen(env_val);
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
		n = strlen(cmdline_argv[argn + cmdline_shift]);
		if (n > space_left)
			return ENOROOM;
		arg = cmdline_argv[argn + cmdline_shift];
		while (*arg != '\0')/* ************************************************************************** */
		/*                                                                            */
		/*                                                        :::      ::::::::   */
		/*   expand.c                                           :+:      :+:    :+:   */
		/*                                                    +:+ +:+         +:+     */
		/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
		/*                                                +#+#+#+#+#+   +#+           */
		/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
		/*   Updated: 2025/04/22 11:47:44 by omaezzem         ###   ########.fr       */
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
		
		char* gitenv(const char* name) {
			return getenv(name);
		}
		
		int expand_env(char **oldp, char **newp, int space_left, int brace_flag)
		{
			int i = 0, n = 0;
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
		
			// Copy env_val to new string if exists
			if (env_val != NULL)
			{
				n = strlen(env_val);
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
			if ((argn + cmdline_shift) < (cmdline_ar		parsing/utils_prs/append_token.c
				gc))
			{
				n = strlen(cmdline_argv[argn + cmdline_shift]);
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
			if ((n = strlen(pid_buffer)) > space_left)
				return ENOROOM;
			i = 0;
			while (pid_buffer[i] != '\0')
				*new++ = pid_buffer[i++];
			*newp = new;
			return n;
		}
		
		int expand(t_token *token)
		{
			char *old = token->value;
			t_env *env = malloc(sizeof(t_env)); // Allocate memory for the env variable
			int oldlen = strlen(old);
			int newsize = oldlen * 2;
			char *new = malloc(newsize);
			if (!new) return -1;
		
			char *newp = new;
			int rv;
		
			while (*old != '\0') {
				if (*old == '$') {
					old++;
					if (*old == '\0')
						return 0; // No variable name after $
					if (*old == '$') {
						old++;
						rv = expand_pid(&newp, newsize - (newp - new));
						if (rv < 0)
							return 0;/* ************************************************************************** */
							/*                                                                            */
							/*                                                        :::      ::::::::   */
							/*   expand.c                                           :+:      :+:    :+:   */
							/*                                                    +:+ +:+         +:+     */
							/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
							/*                                                +#+#+#+#+#+   +#+           */
							/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
							/*   Updated: 2025/04/22 11:47:44 by omaezzem         ###   ########.fr       */
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
							
							char* gitenv(const char* name) {
								return getenv(name);
							}
							
							int expand_env(char **oldp, char **newp, int space_left, int brace_flag)
							{
								int i = 0, n = 0;
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
							
								// Copy env_val to new string if exists
								if (env_val != NULL)
								{
									n = strlen(env_val);
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
								if ((argn + cmdline_shift) < (cmdline_ar		parsing/utils_prs/append_token.c
									gc))
								{
									n = strlen(cmdline_argv[argn + cmdline_shift]);
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
								if ((n = strlen(pid_buffer)) > space_left)
									return ENOROOM;
								i = 0;
								while (pid_buffer[i] != '\0')
									*new++ = pid_buffer[i++];
								*newp = new;
								return n;
							}
							
							int expand(t_token *token)
							{
								char *old = token->value;
								t_env *env = malloc(sizeof(t_env)); // Allocate memory for the env variable
								int oldlen = strlen(old);
								int newsize = oldlen * 2;
								char *new = malloc(newsize);
								if (!new) return -1;
							
								char *newp = new;
								int rv;
							
								while (*old != '\0') {
									if (*old == '$') {
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
										else {
											int brace_flag = (*old == '{');
											rv = expand_env(&old, &newp, newsize - (newp - new), brace_flag);
											if (rv < 0)
												return 0;
											old++; // advance past the variable name / closing brace
										}
									}
									else {
										*newp++ = *old++;
									}
								}
							
								*newp = '\0';
							
								free(token->value);
								env->value = new;
								printf("Expanded value: %s\n", env->value); // Print the expanded value
								return 1;
							}
							
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
							return 0;/* ************************************************************************** */
							/*                                                                            */
							/*                                                        :::      ::::::::   */
							/*   expand.c                                           :+:      :+:    :+:   */
							/*                                                    +:+ +:+         +:+     */
							/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
							/*                                                +#+#+#+#+#+   +#+           */
							/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
							/*   Updated: 2025/04/22 11:47:44 by omaezzem         ###   ########.fr       */
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
							
							char* gitenv(const char* name) {
								return getenv(name);
							}
							
							int expand_env(char **oldp, char **newp, int space_left, int brace_flag)
							{
								int i = 0, n = 0;
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
							
								// Copy env_val to new string if exists
								if (env_val != NULL)
								{
									n = strlen(env_val);
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
								if ((argn + cmdline_shift) < (cmdline_ar		parsing/utils_prs/append_token.c
									gc))
								{
									n = strlen/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/04/22 11:47:44 by omaezzem         ###   ########.fr       */
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

char* gitenv(const char* name) {
	return getenv(name);
}

int expand_env(char **oldp, char **newp, int space_left, int brace_flag)
{
	int i = 0, n = 0;
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

	// Copy env_val to new string if exists
	if (env_val != NULL)
	{
		n = strlen(env_val);
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
	if ((argn + cmdline_shift) < (cmdline_ar		parsing/utils_prs/append_token.c
		gc))
	{
		n = strlen(cmdline_argv[argn + cmdline_shift]);
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
	if ((n = strlen(pid_buffer)) > space_left)
		return ENOROOM;
	i = 0;
	while (pid_buffer[i] != '\0')
		*new++ = pid_buffer[i++];
	*newp = new;
	return n;
}

int expand(t_token *token)
{
	char *old = token->value;
	t_env *env = malloc(sizeof(t_env)); // Allocate memory for the env variable
	int oldlen = strlen(old);
	int newsize = oldlen * 2;
	char *new = malloc(newsize);
	if (!new) return -1;

	char *newp = new;
	int rv;

	while (*old != '\0') {
		if (*old == '$') {
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
			else {
				int brace_flag = (*old == '{');
				rv = expand_env(&old, &newp, newsize - (newp - new), brace_flag);
				if (rv < 0)
					return 0;
				old++; // advance past the variable name / closing brace
			}
		}
		else {
			*newp++ = *old++;
		}
	}

	*newp = '\0';

	free(token->value);
	env->value = new;
	printf("Expanded value: %s\n", env->value); // Print the expanded value
	return 1;
}
(cmdline_argv[argn + cmdline_shift]);
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
								if ((n = strlen(pid_buffer)) > space_left)
									return ENOROOM;
								i = 0;
								while (pid_buffer[i] != '\0')
									*new++ = pid_buffer[i++];
								*newp = new;
								return n;
							}
							
							int expand(t_token *token)
							{
								char *old = token->value;
								t_env *env = malloc(sizeof(t_env)); // Allocate memory for the env variable
								int oldlen = strlen(old);
								int newsize = oldlen * 2;
								char *new = malloc(newsize);
								if (!new) return -1;
							
								char *newp = new;
								int rv;
							
								while (*old != '\0') {
									if (*old == '$') {
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
										else {
											int brace_flag = (*old == '{');
											rv = expand_env(&old, &newp, newsize - (newp - new), brace_flag);
											if (rv < 0)
												return 0;
											old++; // advance past the variable name / closing brace
										}
									}
									else {
										*newp++ = *old++;
									}
								}
							
								*newp = '\0';
							
								free(token->value);
								env->value = new;
								printf("Expanded value: %s\n", env->value); // Print the expanded value
								return 1;
							}
							
						old++; // move past the digit(s) already handled
					}
					else {
						int brace_flag = (*old == '{');
						rv = expand_env(&old, &newp, newsize - (newp - new), brace_flag);
						if (rv < 0)
							return 0;
						old++; // advance past the variable name / closing brace
					}
				}
				else {
					*newp++ = *old++;
				}
			}
		
			*newp = '\0';
		
			free(token->value);
			env->value = new;
			printf("Expanded value: %s\n", env->value); // Print the expanded value
			return 1;
		}
		
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
	if ((n = strlen(pid_buffer)) > space_left)
		return ENOROOM;
	i = 0;
	while (pid_buffer[i] != '\0')
		*new++ = pid_buffer[i++];
	*newp = new;
	return n;
}

int expand(t_token *token)
{
	char *old = token->value;
	t_env *env = malloc(sizeof(t_env)); // Allocate memory for the env variable
	int oldlen = strlen(old);
	int newsize = oldlen * 2;
	char *new = malloc(newsize);
	if (!new) return -1;

	char *newp = new;
	int rv;

	while (*old != '\0') {
		if (*old == '$') {
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
			else {
				int brace_flag = (*old == '{');
				rv = expand_env(&old, &newp, newsize - (newp - new), brace_flag);
				if (rv < 0)
					return 0;
				old++; // advance past the variable name / closing brace
			}
		}
		else {
			*newp++ = *old++;
		}
	}

	*newp = '\0';

	free(token->value);
	env->value = new;
	printf("Expanded value: %s\n", env->value); // Print the expanded value
	return 1;
}

		char *cmd = NULL, *opt = NULL, *redir = NULL;
		while (tokens && tokens->type != TOKEN_PIPE)
		{
			if (tokens->type == TOKEN_WORD)
				add_token_string(&cmd, tokens->value);
			else if (tokens->type == TOKEN_OPTION)
				add_token_string(&opt, tokens->value);
			else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
			         tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
				handle_redirection(&tokens, &redir);
			tokens = tokens->next;
		}
		t_cmd *node = create_command_block(cmd, opt, redir);
		free(cmd); free(opt); free(redir);
		if (!head) head = node;
		else last->next = node;
		last = node;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	print_command_blocks(head);
}
