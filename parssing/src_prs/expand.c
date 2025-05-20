/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/20 14:03:33 by mel-badd         ###   ########.fr       */
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

int expand_env(char **oldp, char **newp, int brace_flag, char *var, t_env *env)
{
    int i = 0, n = 0;
    size_t space_left = 0;
    char *env_val;
    char *old = *oldp;
    char *new = *newp;
    // printf("old: %s\n", old);
    // var = ft_strcpy(var, old);
    // If it's an unbraced variable, process it accordingly

    if (!brace_flag)
    {
        // Parse valid var name characters (letters, digits, underscores)
        while (ft_isalnum(*old) || *old == '_')
            var[i++] = *old++;
    }
    else
    {
        // Handle braced variables
        old++;  // skip '{'
        while (*old != '}' && *old != '\0')
        {
            var[i++] = *old++;
        }
        if (*old == '\0')  // Unmatched closing brace
        {
            return ENOBRACE;  // Error: unmatched brace
        }
        old++;  // skip '}'
    }
    var[i] = '\0';


	// while (env)
    // {
    //     printf("--->%s=%s\n", env->var, env->val);
    //     // printf("val: %s\n", env->val);
    //     env = env->next;
    // }
    // printf("i: %d\n", i);
    // 		    while (env)
    // {
    //     printf("var: %s\n", env->var);
    //     printf("val: %s\n", env->val);
    //     env = env->next;
    // }

    // Fetch environment variable
    // printf("varjkh: %s\n", env->var);
    // 		    while (env)
    // {
    //     printf("var: %s\n", env->var);
    //     printf("val: %s\n", env->val);
    //     env = env->next;
    // }
    // printf("USER: %s\n", getenv("USER"));


    // printf("varjg: %s\n", var);
    env_val = find_env(env, var);
    if (env_val == NULL) {
        env_val = "";  // If not found, treat as empty string
        space_left = 1;  // Only need space for an empty string
    } else {
        space_left = ft_strlen(env_val);  // Space required for the variable value
    }

    // Copy the environment variable value into the new buffer
    while (*env_val != '\0' && space_left-- > 0) {
        *new++ = *env_val++;  // Copy each character from env_val to new
    }

    // Null-terminate the string after copying
    *new = '\0';

    // Update the pointers to continue processing
    *oldp = old;  // Move old pointer to the next character after processed variable
    *newp = new;  // Update new pointer

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
#include <stdlib.h>
#include <string.h>

char *append_to_result(char *final, char *part, size_t total_len, size_t part_len) {
    // Allocate memory for the new string (current total length + part length + 1 for null-terminator)
    char *temp = malloc(total_len + part_len + 1);
    if (!temp) return NULL;  // Return NULL if memory allocation fails

    // Copy the contents of the current final string into temp (if final is not NULL)
    if (final) {
        memcpy(temp, final, total_len);  // Copy the total_len bytes from final into temp
        free(final);  // Free the old final string to avoid memory leak
    }

    // Copy the contents of the part string into temp, starting after the existing final string
    memcpy(temp + total_len, part, part_len);  // Copy part_len bytes from part into temp

    // Null-terminate the new string
    temp[total_len + part_len] = '\0';

    return temp;  // Return the newly concatenated string
}

// int handle_export(t_token )
char *expand(t_token *token, t_env *env)
{
    char *old;
    int oldlen;
    int newsize;
    char *new = NULL;
    char *newp;
    int rv;

    char *final_result = NULL;
    size_t total_len = 0;

    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd) return 0;

    cmd->env = malloc(sizeof(t_env));
    if (!cmd->env) {
        free(cmd);
        return 0;
    }

    t_token *tmp_token = token;
    // while (token)
    // {
    //     if (token->type == TOKEN_PIPE)
    //         tmp_token = token->next;
    //     token = token->next;
    // }
    // Skip command name if needed
    // if (tmp_token && tmp_token->type == TOKEN_WORD)
    //     tmp_token = tmp_token->next;

    int i = 0;

    while (tmp_token)
    {
        // printf("tmp_token->value number(%d): %s\n", i, tmp_token->value);
        // printf("tmp_token->value number(%d): %s\n", i,tmp_token->value);
        i++;

        if (tmp_token->type == TOKEN_SPACE) {
            // Avoid adding multiple spaces by checking the last added character
            if (final_result && final_result[total_len - 1] != ' ') {
                // Add a single space if there isn't already one
                final_result = append_to_result(final_result, " ", total_len, 1);
                total_len += 1;
            }
            tmp_token = tmp_token->next;
            continue;
        }

        old = tmp_token->value;
        oldlen = ft_strlen(old);
        newsize = oldlen * 2 + 1;
        new = malloc(newsize);
        if (!new) {
            // perror("malloc failed");
            // free(final_result);
            // free(cmd->env);
            // free(cmd);
            return 0;
        }

        newp = new;
        int in_single = 0;
        int in_double = 0;

        while (*old != '\0') {
            if (*old == '\'' && !in_double) {
                in_single = !in_single;
                *newp++ = *old++;
                // old++;
            } else if (*old == '"' && !in_single) {
                in_double = !in_double;
                *newp++ = *old++;
                // old++;
            } else if (*old == '$' && !in_single) {
                old++;
                if (*old == '\0') break;

                if (*old == '$') {
                    old++;
                    // Add PID logic if needed
                } else if (*old == '?') {
                    old++;
                    rv = expand_status(&newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                } else if (*old == '#') {
                    old++;
                    rv = expand_argc(&newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                } else if (isdigit(*old)) {
                    rv = expand_argv(&old, &newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                    old++;
                } else {
                    int brace_flag = (*old == '{');
                    char var[256];
                    rv = expand_env(&old, &newp, brace_flag, var, env);
                    if (rv < 0) return 0;
                }
            } else {
                *newp++ = *old++;
            }
        }

        *newp = '\0';

        size_t part_len = ft_strlen(new);
        char *temp = malloc(total_len + part_len + 1);
        if (!temp) {
            // free(new);
            // free(final_result);
            // free(cmd->env);
            // free(cmd);
            return 0;
        }

        if (final_result) {
            ft_memcpy(temp, final_result, total_len);
            free(final_result);
        }

        ft_memcpy(temp + total_len, new, part_len);
        total_len += part_len;
        temp[total_len] = '\0';
        final_result = temp;

        // free(new);
        tmp_token = tmp_token->next;
    }

    cmd->value_expand = final_result;
    // printf("Expanded value: %s\n", cmd->value_expand);
    // printf("Expanded value: %s\n", cmd->env->val);
    return cmd->value_expand;
}
