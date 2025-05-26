/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:00:24 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 23:42:16 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <string.h>

extern int prev_status;
// static char *append_to_result(char *final, const char *part, size_t total_len) {
//     size_t part_len = ft_strlen_(part);
//     char *t = malloc(total_len + part_len + 1);
//     if (!t) return NULL;
//     if (final) {
//         ft_memcpy_(t, final, total_len);
//         free(final);
//     }
//     // copy part + null
//     ft_memcpy_(t + total_len, part, part_len + 1);
//     return t;
// }

// find in linked-list env
// static char *find_env(t_env *e, const char *key) {
//     while (e) {
//         if (strcmp(e->var, key) == 0)
//             return e->val;
//         e = e->next;
//     }
//     return NULL;
// }

// ——— Single‐char handlers —————————————————————————

int expand_status(char **newp, int space_left) {
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

int expand_argv(char **oldp, char **newp, int space_left) {
    char arg_idx[4], *arg;
    char *old = *oldp;
    char *new = *newp;
    int argn, n = 0, i = 0;
    while (isdigit(*old) && i < 3)
        arg_idx[i++] = *old++;
    arg_idx[i] = '\0';
    argn = atoi(arg_idx);
    if ((argn + cmdline_shift) < cmdline_argc) {
        n = ft_strlen(cmdline_argv[argn + cmdline_shift]);
        if (n > space_left)
            return ENOROOM;
        arg = cmdline_argv[argn + cmdline_shift];
        while (*arg != '\0')
            *new++ = *arg++;
    }
    *oldp = old - 1;
    *newp = new;
    return n;
}
int expand_env(char **oldp, char **newp, int brace_flag, char *var, t_env *env) {
    char *old = *oldp;
    char *new = *newp;
    int  i = 0;

    // 1) parse NAME or {NAME}
    if (!brace_flag) {
        while (ft_isalnum(*old) || *old == '_') {
            if (i >= 255) return ENOROOM;
            var[i++] = *old++;
        }
    } else {
        old++; // skip '{'
        while (*old && *old != '}') {
            if (i >= 255) return ENOROOM;
            var[i++] = *old++;
        }
        if (!*old) return ENOBRACE;
        old++; // skip '}'
    }
    var[i] = '\0';

    // 2) lookup
    char *env_val = find_env(env, var);
    if (!env_val) env_val = "";

    // 3) copy ALL characters + null
    size_t len = ft_strlen(env_val);
    for (size_t j = 0; j < len; j++)
        *new++ = env_val[j];
    *new++ = '\0';

    // 4) update pointers
    *newp = new;
    *oldp = old;
    return 0;
}


int expand_argc(char **newp, int space_left) {
    char argc_buffer[10], *new = *newp;
    int n, i = 0;
    if (cmdline_argc < cmdline_shift)
        n = snprintf(argc_buffer, 10, "0");
    else
        n = snprintf(argc_buffer, 10, "%d", cmdline_argc - cmdline_shift);
    if (n > space_left)
        return ENOROOM;
    while (argc_buffer[i] != '\0')
        *new++ = argc_buffer[i++];
    *newp = new;
    return n;
}
int expand_pid(char **newp, int space_left) {
    int i = 0, n;
    pid_t pid = getpid();
    char pid_buffer[10];
    char *new = *newp;
    n = snprintf(pid_buffer, 10, "%d", pid);
    if (n > space_left)
        return ENOROOM;
    while (pid_buffer[i] != '\0')
        *new++ = pid_buffer[i++];
    *newp = new;
    return n;
}

// ——— Full expand routine ——————————————————————————

static char *append_to_result(char *final, const char *part, size_t total_len) {
    size_t part_len = ft_strlen((char *)part);
    char *new = malloc(total_len + part_len + 1);
    if (!new) return NULL;
    if (final) {
        ft_memcpy(new, final, total_len);
        free(final);
    }
    ft_memcpy(new + total_len, part, part_len + 1);
    return new;
}

// Main expansion: iterates tokens, expands variables, and builds one string
char *expand(t_token *token, t_env *env) {
    char *result = NULL;
    size_t total = 0;

    while (token) {
        char *src = token->value;
        int in_single = 0, in_double = 0;

        while (*src) {
            if (*src == '\'' && !in_double) {
                in_single = !in_single;
                result = append_to_result(result, "'", total++);
                src++;
                continue;
            }
            if (*src == '\"' && !in_single) {
                in_double = !in_double;
                result = append_to_result(result, "\"", total++);
                src++;
                continue;
            }

            if (*src == '$' && !in_single) {
                // Count consecutive $ signs
                int dollar_count = 0;
                while (*src == '$') {
                    dollar_count++;
                    src++;
                }

                int escape_count = dollar_count / 2;
                int expand = dollar_count % 2;

                // Append escaped $'s
                for (int i = 0; i < escape_count; i++) {
                    result = append_to_result(result, "$", total);
                    total++;
                }

                if (!expand) continue; // if even, no expansion

                // Now parse variable name for expansion
                char varname[256] = {0};
                int brace = (*src == '{');
                int valid = 0;

                if (brace) {
                    src++;  // skip {
                    int i = 0;
                    while (*src && *src != '}' && i < 255)
                        varname[i++] = *src++;
                    varname[i] = '\0';
                    if (*src == '}') src++;  // skip }
                    valid = i > 0;
                } else if (*src == '?' || *src == '#' || *src == '$' || isdigit(*src) || ft_isalpha(*src) || *src == '_') {
                    int i = 0;
                    while ((ft_isalnum(*src) || *src == '_') && i < 255)
                        varname[i++] = *src++;
                    varname[i] = '\0';
                    valid = i > 0;
                }

                if (!valid) {
                    // Invalid var: just output one $
                    result = append_to_result(result, "$", total);
                    total++;
                    continue;
                }

                // Expand the var
                char *val = NULL;
                if (strcmp(varname, "?") == 0) {
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%d", prev_status);
                    val = buf;
                } else if (strcmp(varname, "#") == 0) {
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%d", cmdline_argc - cmdline_shift);
                    val = buf;
                } else if (strcmp(varname, "$") == 0) {
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%d", getpid());
                    val = buf;
                } else if (isdigit(varname[0])) {
                    int idx = atoi(varname) + cmdline_shift;
                    val = (idx < cmdline_argc) ? cmdline_argv[idx] : "";
                } else {
                    val = find_env(env, varname);
                    if (!val) val = "";
                }

                result = append_to_result(result, val, total);
                total += ft_strlen(val);
                continue;
            }

            // Regular character
            char tmp[2] = {*src, '\0'};
            result = append_to_result(result, tmp, total);
            total++;
            src++;
        }

        // Add space after token if next is TOKEN_SPACE
        if (token->next && token->next->type == TOKEN_SPACE) {
            result = append_to_result(result, " ", total);
            total++;
        }

        token = token->next;
    }

    return result;
}
