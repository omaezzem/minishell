/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/03 15:58:17 by omaezzem         ###   ########.fr       */
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
    // If it's an unbraced variable, process it accordingly
    if (!brace_flag)
    {
        // Parse valid var name characters (letters, digits, underscores)
        while (isalnum(*old) || *old == '_')
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
    var[i] = '\0';  // Null-terminate the variable name
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
    env_val = find_env(env, var);
    printf("Looking for var: %s, found value: %s\n", var, env_val);
	// while (env)
    // {
    //     printf("var: %s\n", env->var);
    //     printf("val: %s\n", env->val);
    //     env = env->next;
    // }
	// 	printf("env_val: %s\n", env_val);
    if (env_val == NULL)
    {
        // No such variable, return empty string
        env_val = "";
        space_left = 1;  // We can store an empty string
    }
    else
    {
        space_left = ft_strlen(env_val);
    }

    // Check if there is enough space in the destination buffer
    if (space_left < ft_strlen(env_val))
    {
        return ENOROOM;  // Error: Not enough space
    }

    // Copy the environment variable to the new string if it exists
    while (*env_val != '\0')
    {
        *new++ = *env_val++;
    }

    // Update the pointers
    *oldp = old;  // Move old pointer to the next character after processed variable
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
	argn = mini_atoi(arg_idx);
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

int expand(t_token *token, t_env *env)
{
    char *old = NULL;
    int oldlen = 0;
    int newsize = 0;
    char *new = NULL;
    t_token *tmp_token = token;
    char *newp;
    int rv;
    t_cmd *cmd = malloc(sizeof(t_cmd));  // Ensure cmd is allocated
    if (!cmd) return 0;


    cmd->env = malloc(sizeof(t_env));  // Ensure env in cmd is allocated
    if (!cmd->env) {
        free(cmd);  // Free cmd if env allocation fails
        return 0;
    }
	// char 	*find;
    // printf("env: %s\n", env->next->var);
	// find = find_env(env, "USER");
	// printf("%s\n", find);
    // Initialize cmd->env to a safe state
    // cmd->env->var = NULL;
    // cmd->env->val = NULL;
    // cmd->env->next = NULL;
    // Iterate through all tokens and expand them
    while (tmp_token)
    {
        old = tmp_token->value;
        oldlen = ft_strlen(old);
        newsize = oldlen * 2;
        new = malloc(newsize + 1);  // Allocate memory for the expanded string
        if (!new) {
            free(cmd->env);
            free(cmd);
            return 0;  // Memory allocation failed
        }
        newp = new;

        while (*old != '\0')
        {
            if (*old == '"') {
                old++;  // Skip over quotes
            }
            else if (*old == '$') {
                old++;  // Skip over the dollar sign
                if (*old == '\0') return 0;  // No variable after $

                // Handle variable expansion inside the quotes
                if (*old == '$') {
                    old++;
                    // rv = expand_pid(&newp, newsize - (newp - new));
                    // if (rv < 0) return 0;
                }
                else if (*old == '?') {
                    old++;
                    rv = expand_status(&newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                }
                else if (*old == '#') {
                    old++;
                    rv = expand_argc(&newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                }
                else if (isdigit(*old)) {
                    rv = expand_argv(&old, &newp, newsize - (newp - new));
                    if (rv < 0) return 0;
                    old++; // move past the digit(s) already handled
                }
                else {
                    int brace_flag = (*old == '{');
                    // env = env->next;
                    // printf("var: %s\n", env->var);
                    rv = expand_env(&old, &newp, brace_flag, env->var, env);

                    if (rv < 0) return 0;
					old++; // move past the variable name
                }
            }
            else {
                *newp++ = *old++;  // Copy normal character
            }
        }

        *newp = '\0';  // Null-terminate the expanded string

        // Assign the expanded value to cmd->env->val
        // printf("Expanded value: %s\n", new);  // Print the expanded value
        	    // cmd->env->val = ft_strdup(new);
        	    // printf("cmd->env->val: %s\n", cmd->env->val);
        	    // free(cmd->env->val);
        	    //
        cmd->env->val = new;  // Store the expanded string
        	    // printf("here\n");

        tmp_token = tmp_token->next;
    }
        printf("Expanded value: %s\n", cmd->env->val);  // Print the expanded value

    return 1;
}

// int expand(t_token *token)
// {
//     char *old = NULL;
//     int oldlen = 0;
//     int newsize = 0;
//     char *new = NULL;
//     t_token *tmp_token = token;
//     char *newp;
//     int rv;
//     t_cmd *cmd = malloc(sizeof(t_cmd));  // Ensure cmd is allocated
//     if (!cmd) return 0;

//     cmd->env = malloc(sizeof(t_env));  // Ensure env in cmd is allocated
//     if (!cmd->env) {
//         free(cmd);  // Free cmd if env allocation fails
//         return 0;
//     }

//     // Initialize cmd->env to a safe state
//     cmd->env->var = NULL;
//     cmd->env->val = NULL;
//     cmd->env->next = NULL;

//     // Iterate through all tokens and expand them
//     while (tmp_token)
//     {
//         old = tmp_token->value;
//         oldlen = ft_strlen(old);
//         newsize = oldlen * 2;
//         new = malloc(newsize + 1);  // Allocate memory for the expanded string
//         if (!new) {
//             free(cmd->env);
//             free(cmd);
//             return 0;  // Memory allocation failed
//         }
//         newp = new;

//         while (*old != '\0')
//         {
//             if (*old == '"') {
//                 old++;  // Skip over quotes
//             }
//             else if (*old == '$') {
//                 old++;  // Skip over the dollar sign
//                 if (*old == '\0') return 0;  // No variable after $

//                 // Handle different cases after $ symbol
//                 if (*old == '$') {
//                     old++;
//                     rv = expand_pid(&newp, newsize - (newp - new));
//                     if (rv < 0) return 0;
//                 }
//                 else if (*old == '?') {
//                     old++;
//                     rv = expand_status(&newp, newsize - (newp - new));
//                     if (rv < 0) return 0;
//                 }
//                 else if (*old == '#') {
//                     old++;
//                     rv = expand_argc(&newp, newsize - (newp - new));
//                     if (rv < 0) return 0;
//                 }
//                 else if (isdigit(*old)) {
//                     rv = expand_argv(&old, &newp, newsize - (newp - new));
//                     if (rv < 0) return 0;
//                 }
//                 else {
//                     int brace_flag = (*old == '{');
//                     rv = expand_env(&old, &newp, brace_flag);
//                     if (rv < 0) return 0;
//                 }
//             }
//             else {
//                 *newp++ = *old++;  // Copy normal character
//             }
//         }

//         *newp = '\0';  // Null-terminate the expanded string

//         // Assign the expanded value to cmd->env->val
//         cmd->env->val = new;  // Store the expanded string
//         printf("Expanded value: %s\n", cmd->env->val);  // Print the expanded value

//         tmp_token = tmp_token->next;
//     }

//     return 1;
// }
