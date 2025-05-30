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
// #include <stdio.h>
// #include <string.h>
// #include "../../include/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

extern int prev_status;

// typedef struct s_dynstr {
//     char *data;
//     size_t len;
//     size_t cap;
// } t_dynstr;

// static void dyn_str_init(t_dynstr *ds) {
//     ds->data = NULL;
//     ds->len = 0;
//     ds->cap = 0;
// }

// static int dyn_str_append(t_dynstr *ds, const char *str, size_t len) {
//     if (!str || len == 0) return 0;
//     if (ds->cap < ds->len + len + 1) {
//         size_t new_cap = ds->cap ? ds->cap * 2 : 256;
//         while (new_cap < ds->len + len + 1)
//             new_cap *= 2;
//         char *new_data = realloc(ds->data, new_cap);
//         if (!new_data) return -1;
//         ds->data = new_data;
//         ds->cap = new_cap;
//     }
//     memcpy(ds->data + ds->len, str, len);
//     ds->len += len;
//     return 0;
// }

// static char *dyn_str_finalize(t_dynstr *ds) {
//     if (!ds) return NULL;
//     if (ds->cap < ds->len + 1) {
//         char *new_data = realloc(ds->data, ds->len + 1);
//         if (!new_data) return NULL;
//         ds->data = new_data;
//         ds->cap = ds->len + 1;
//     }
//     ds->data[ds->len] = '\0';
//     return ds->data;
// }

// static char *expand_token_string(char *src, t_env *env) {
//     t_dynstr ds;
//     dyn_str_init(&ds);
//     int in_single = 0, in_double = 0;

//     while (src && *src) {
//         // Handle quotes
//         if (*src == '\'' && !in_double) {
//             in_single = !in_single;
//             dyn_str_append(&ds, "'", 1);
//             src++;
//             continue;
//         }
        
//         if (*src == '\"' && !in_single) {
//             in_double = !in_double;
//             dyn_str_append(&ds, "\"", 1);
//             src++;
//             continue;
//         }

//         // Handle variable expansion
//         if (*src == '$' && !in_single) {
//             int count = 0;
//             while (src[count] == '$') count++;
//             int escape_count = count / 2;
//             int expand = count % 2;

//             // Add escaped $
//             for (int i = 0; i < escape_count; i++)
//                 dyn_str_append(&ds, "$", 1);

//             src += count;
//             if (!expand || !*src) continue;

//             char var[256] = {0};
//             int brace = (*src == '{');
//             int valid = 0;

//             // Parse variable name
//             if (brace) {
//                 src++;
//                 char *end = strchr(src, '}');
//                 if (!end) {
//                     dyn_str_append(&ds, "${", 2);
//                     continue;
//                 }
//                 strncpy(var, src, end - src);
//                 src = end + 1;
//                 valid = 1;
//             } 
//             else if (*src == '?' || *src == '$' || *src == '#' || isdigit(*src) || isalpha(*src) || *src == '_') {
//                 int i = 0;
//                 // Single-char special vars
//                 if (*src == '?' || *src == '$' || *src == '#') {
//                     var[i++] = *src++;
//                 } 
//                 // Numeric params
//                 else if (isdigit(*src)) {
//                     while (isdigit(*src) && i < 255)
//                         var[i++] = *src++;
//                 } 
//                 // Regular vars
//                 else {
//                     while ((isalnum(*src) || *src == '_') && i < 255)
//                         var[i++] = *src++;
//                 }
//                 var[i] = '\0';
//                 valid = 1;
//             }

//             if (!valid) {
//                 dyn_str_append(&ds, "$", 1);
//                 continue;
//             }

//             // Handle special variables
//             char *val = NULL;
//             if (strcmp(var, "?") == 0) {
//                 char buf[20];
//                 snprintf(buf, sizeof(buf), "%d", prev_status);
//                 val = buf;
//             } 
//             else if (strcmp(var, "#") == 0) {
//                 char buf[20];
//                 snprintf(buf, sizeof(buf), "%d", cmdline_argc - cmdline_shift);
//                 val = buf;
//             } 
//             else if (strcmp(var, "$") == 0) {
//                 char buf[20];
//                 snprintf(buf, sizeof(buf), "%d", getpid());
//                 val = buf;
//             } 
//             else if (isdigit(var[0])) {
//                 int idx = atoi(var);
//                 if (idx + cmdline_shift < cmdline_argc)
//                     val = cmdline_argv[idx + cmdline_shift];
//                 else
//                     val = "";
//             } 
//             // Regular environment variable
//             else {
//                 val = find_env(env, var);
//                 if (!val) val = "";
//             }

//             dyn_str_append(&ds, val, strlen(val));
//         } 
//         // Regular character
//         else {
//             char ch[2] = {*src, '\0'};
//             dyn_str_append(&ds, ch, 1);
//             src++;
//         }
//     }
//     return dyn_str_finalize(&ds);
// }
// char *expand(t_token *token, t_env *env) {
//     t_dynstr ds;
//     dyn_str_init(&ds);

//     while (token) {
//         char *expanded = NULL;
//         if (token->type == TOKEN_WORD) {
//             expanded = expand_token_string(token->value, env);
//             if (!expanded) expanded = strdup("");
//             dyn_str_append(&ds, expanded, strlen(expanded));
//             free(expanded);
//         } else if (token->type == TOKEN_SPACE) {
//             dyn_str_append(&ds, " ", 1);
//         } else {
//             dyn_str_append(&ds, token->value, strlen(token->value));
//         }
//         token = token->next;
//     }

//     if (ds.cap == 0) {
//         ds.data = malloc(1);
//         if (ds.data) ds.data[0] = '\0';
//     } else {
//         ds.data[ds.len] = '\0';
//     }
//     return ds.data;
// }
// extern int prev_status;
static char *append_to_result(char *final, char *part, size_t total_len) {
    size_t part_len = ft_strlen(part);
    char *t = malloc(total_len + part_len + 1);
    if (!t) return NULL;
    if (final) {
        ft_memcpy(t, final, total_len);
        free(final);
    }
    // copy part + null
    ft_memcpy(t + total_len, part, part_len + 1);
    return t;
}

// // find in linked-list env
// // static char *find_env(t_env *e, const char *key) {
// //     while (e) {
// //         if (strcmp(e->var, key) == 0)
// //             return e->val;
// //         e = e->next;
// //     }
// //     return NULL;
// // }

// // ——— Single‐char handlers —————————————————————————

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

// static char *append_to_result(char *final, const char *part, size_t total_len) {
//     size_t part_len = ft_strlen((char *)part);
//     char *new = malloc(total_len + part_len + 1);
//     if (!new) return NULL;
//     if (final) {
//         ft_memcpy(new, final, total_len);
//         free(final);
//     }
//     ft_memcpy(new + total_len, part, part_len + 1);
//     return new;
// }

// Main expansion: iterates tokens, expands variables, and builds one string
// static char *find_env(t_env *env, const char *var) {
//     while (env) {
//         if (strcmp(env->var, var) == 0)
//             return env->val;
//         env = env->next;
//     }
//     return NULL;
// }
typedef struct s_dynstr {
    char *data;
    size_t len;
    size_t cap;
} t_dynstr;

void dyn_str_init(t_dynstr *ds) {
    ds->data = NULL;
    ds->len = 0;
    ds->cap = 0;
}

int dyn_str_append(t_dynstr *ds, const char *str, size_t len) {
    if (!str || len == 0) return 0;
    if (ds->cap < ds->len + len + 1) {
        size_t new_cap = ds->cap ? ds->cap * 2 : 256;
        while (new_cap < ds->len + len + 1) new_cap *= 2;
        char *new_data = realloc(ds->data, new_cap);
        if (!new_data) return -1;
        ds->data = new_data;
        ds->cap = new_cap;
    }
    memcpy(ds->data + ds->len, str, len);
    ds->len += len;
    ds->data[ds->len] = '\0';
    return 0;
}

char *dyn_str_finalize(t_dynstr *ds) {
    if (!ds) return NULL;
    if (ds->cap < ds->len + 1) {
        char *new_data = realloc(ds->data, ds->len + 1);
        if (!new_data) return NULL;
        ds->data = new_data;
        ds->cap = ds->len + 1;
    }
    ds->data[ds->len] = '\0';
    return ds->data;
}
t_token *expand(t_token *token, t_env *env) {
    t_token *result = NULL;
    t_token **current = &result;

    while (token) {
        t_token *next = token->next;
        token->next = NULL;

        if (token->type == TOKEN_WORD && strchr(token->value, '$')) {
            char *src = token->value;
            int in_single = 0, in_double = 0;
            t_dynstr ds = {0};

            while (*src) {
                if (*src == '\'' && !in_double) {
                    in_single = !in_single;
                    dyn_str_append(&ds, "'", 1);
                    src++;
                    continue;
                }

                if (*src == '"' && !in_single) {
                    in_double = !in_double;
                    dyn_str_append(&ds, "\"", 1);
                    src++;
                    continue;
                }

                if (*src == '$' && !in_single) {
                    int dollar_count = 0;
                    char *dollar_start = src;
                    while (*src == '$') {
                        dollar_count++;
                        src++;
                    }

                    int escape_count = dollar_count / 2;
                    int expand = dollar_count % 2;

                    for (int i = 0; i < escape_count; i++) {
                        dyn_str_append(&ds, "$", 1);
                    }

                    if (!expand) continue;

                    char varname[256] = {0};
                    int brace = (*src == '{');
                    int valid = 0;

                    if (brace) {
                        src++;
                        char *end = strchr(src, '}');
                        if (!end) {
                            dyn_str_append(&ds, "${", 2);
                            continue;
                        }
                        strncpy(varname, src, end - src);
                        src = end + 1;
                        valid = 1;
                    } else if (*src == '?' || *src == '#' || *src == '$' || 
                               isdigit(*src) || isalpha(*src) || *src == '_') {
                        char *start = src;
                        while (*src && (isalnum(*src) || *src == '_')) {
                            src++;
                        }
                        strncpy(varname, start, src - start);
                        valid = 1;
                        src--;  // Adjust for loop increment
                    }

                    if (!valid) {
                        dyn_str_append(&ds, "$", 1);
                        src = dollar_start + 1;
                        continue;
                    }

                    char *val = NULL;
                    if (strcmp(varname, "?") == 0) {
                        char buf[20];
                        snprintf(buf, sizeof(buf), "%d", prev_status);
                        val = strdup(buf);
                    } else if (strcmp(varname, "#") == 0) {
                        char buf[20];
                        snprintf(buf, sizeof(buf), "%d", cmdline_argc - cmdline_shift);
                        val = strdup(buf);
                    } else if (isdigit(varname[0])) {
                        int idx = atoi(varname);
                        if (idx + cmdline_shift < cmdline_argc) {
                            val = strdup(cmdline_argv[idx + cmdline_shift]);
                        } else {
                            val = strdup("");
                        }
                    } else {
                        char *env_val = find_env(env, varname);
                        val = env_val ? strdup(env_val) : strdup("");
                    }

                    if (val) {
                        dyn_str_append(&ds, val, strlen(val));
                        free(val);
                    }
                } else {
                    char ch[2] = {*src, '\0'};
                    dyn_str_append(&ds, ch, 1);
                }

                src++;
            }

            char *expanded_value = dyn_str_finalize(&ds);
            free(token->value);
            token->value = expanded_value;

            t_token *new_tokens = tokenize1(expanded_value);

            while (new_tokens) {
                t_token *next_new = new_tokens->next;
                new_tokens->next = NULL;

                *current = new_tokens;
                current = &new_tokens->next;

                new_tokens = next_new;
            }

            // free(token);
        } else {
            *current = token;
            current = &token->next;
        }

        token = next;
    }

    return result;
}


char *expand_herdoc(t_token *token, t_env *env) {
    char *result = NULL;
    size_t total = 0;

    while (token) {
        char *src = token->value;
        int in_single = 0;

        while (*src) {

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
