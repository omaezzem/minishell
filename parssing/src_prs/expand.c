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

// int expand_env(char **oldp, char **newp, int brace_flag, char *var, t_env *env)
// {
//     char *old = *oldp;
//     char *new = *newp;
//     int  i = 0;

//     // 1) parse NAME or {NAME}
//     if (!brace_flag) {
//         while (ft_isalnum(*old) || *old == '_') {
//             if (i >= 255) return ENOROOM;
//             var[i++] = *old++;
//         }
//     } else {
//         old++; // skip '{'
//         while (*old && *old != '}') {
//             if (i >= 255) return ENOROOM;
//             var[i++] = *old++;
//         }
//         if (!*old) return ENOBRACE;
//         old++; // skip '}'
//     }
//     var[i] = '\0';

//     // 2) lookup
//     char *env_val = find_env(env, var);
//     if (!env_val) env_val = "";

//     // 3) copy ALL characters + null
//     size_t len = ft_strlen(env_val);
//     for (size_t j = 0; j < len; j++)
//         *new++ = env_val[j];
//     *new++ = '\0';

//     // 4) update pointers
//     *newp = new;
//     *oldp = old;
//     return 0;
// }


// int expand_argc(char **newp, int space_left) {
//     char argc_buffer[10], *new = *newp;
//     int n, i = 0;
//     if (cmdline_argc < cmdline_shift)
//         n = snprintf(argc_buffer, 10, "0");
//     else
//         n = snprintf(argc_buffer, 10, "%d", cmdline_argc - cmdline_shift);
//     if (n > space_left)
//         return ENOROOM;
//     while (argc_buffer[i] != '\0')
//         *new++ = argc_buffer[i++];
//     *newp = new;
//     return n;
// }
// int expand_pid(char **newp, int space_left) {
//     int i = 0, n;
//     pid_t pid = getpid();
//     char pid_buffer[10];
//     char *new = *newp;
//     n = snprintf(pid_buffer, 10, "%d", pid);
//     if (n > space_left)
//         return ENOROOM;
//     while (pid_buffer[i] != '\0')
//         *new++ = pid_buffer[i++];
//     *newp = new;
//     return n;
// }

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


void dyn_str_init(t_dynstr *ds)
{
    ds->data = NULL;
    ds->len = 0;
    ds->cap = 0;
}

int dyn_str_append(t_gc *gc, t_dynstr *ds, const char *str, size_t len)
{
    char *new_data;
    size_t new_cap;

    if (str == NULL || len == 0)
        return 0;
    if (ds->cap < ds->len + len + 1) {
        new_cap = ds->cap;
        if (new_cap == 0)
            new_cap = 256;
        while (new_cap < ds->len + len + 1)
            new_cap *= 2;
        new_data = gc_malloc(gc, new_cap);   // بدل malloc بـ gc_malloc
        if (new_data == NULL)
            return -1;
        if (ds->data != NULL)
            ft_memcpy(new_data, ds->data, ds->len);
        // لا تحتاج free(ds->data) لأن GC كيتكلف بها
        ds->data = new_data;
        ds->cap = new_cap;
    }
    ft_memcpy(ds->data + ds->len, str, len);
    ds->len += len;
    ds->data[ds->len] = '\0';
    return 0;
}



#include <stdlib.h>
#include <string.h>

char *dyn_str_finalize(t_gc *gc, t_dynstr *ds)
{
    if (!ds) return NULL;

    if (ds->cap < ds->len + 1) {
        char *new_data = gc_malloc(gc, ds->len + 1);  // بدل malloc بـ gc_malloc
        if (!new_data) return NULL;

        ft_memcpy(new_data, ds->data, ds->len);
        // free(ds->data); // ما تحتاجهاش

        ds->data = new_data;
        ds->cap = ds->len + 1;
    }

    ds->data[ds->len] = '\0';
    return ds->data;
}

// int get_last_exit_status(void)
// {
//     char *status = malloc(20);
//     if (!status) return NULL;
//     // snprintf(status, 20, "%d", prev_status);
//     return status;
// }
// #include "minishell.h"
void	handle_quotes(t_gc *gc, char c, int *s, int *d, t_dynstr *ds)
{
	if (c == '\'' && !(*d))
		*s ^= 1;
	else if (c == '"' && !(*s))
		*d ^= 1;
	else
		return ;
	dyn_str_append(gc, ds, &c, 1);
}

char *get_var_name(t_gc *gc, const char **src)
{
    char var[256];
    int i = 0;

    if (**src == '?')
    {
        (*src)++;
        return gc_strdup(gc, "?");  // بدل ft_strdup بـ gc_strdup
    }
    while (**src && (ft_isalnum(**src) || **src == '_') && i < 255)
        var[i++] = *((*src)++);
    var[i] = '\0';
    if (!*var)
        return NULL;
    return gc_strdup(gc, var);
}


char *get_var_value(t_gc *gc, const char **src, t_env *env, t_cmd *cmd)
{
    char *var;
    char *val;
    var = get_var_name(gc, src);
    if (!var)
        return NULL;
    if (ft_strcmp(var, "?") == 0)
    {
        return gc_itoa(gc, cmd->ex_status);
    }
    val = find_env(env, var);
    return gc_strdup(gc, val ? val : "");
}

void	append_dollars(t_gc *gc, t_dynstr *ds, int count)
{
	while (count--)
		dyn_str_append(gc, ds, "$", 1);
}

void	handle_dollar(t_gc *gc, const char **src, t_dynstr *ds,
			t_env *env, t_cmd *cmd)
{
	int			count;
	const char	*start;
	char		*val;

	count = 0;
	start = *src;
	while (**src == '$')
	{
		count++;
		(*src)++;
	}
	append_dollars(gc, ds, count / 2);
	if (count % 2 == 0)
		return ;
	if (!**src || (!ft_isalpha(**src) && **src != '_' && **src != '?'))
	{
		dyn_str_append(gc, ds, "$", 1);
		*src = start + 1;
		return ;
	}
	val = get_var_value(gc, src, env, cmd);
	if (val)
	{
		dyn_str_append(gc, ds, val, ft_strlen(val));
		// free(val);
	}
}
char	*expand_value(t_gc *gc, char *value, t_env *env, t_cmd *cmd)
{
	t_dynstr	ds;
	const char	*src;
	int			s;
	int			d;

	s = 0;
	d = 0;
	src = value;
	dyn_str_init(&ds);
	while (*src)
	{
		if ((*src == '\'' && !d) || (*src == '"' && !s))
			handle_quotes(gc, *src++, &s, &d, &ds);
		else if (*src == '$' && !s)
            handle_dollar(gc, &src, &ds, env, cmd);
		else
        {
            char tmp[1];
            tmp[0] = *src++;
            dyn_str_append(gc, &ds, tmp, 1);
        }
	}
	return (dyn_str_finalize(gc, &ds));
}
t_token	*add_expanded_tokens(t_gc *gc, char *value, t_token **cur)
{
	t_token	*new_tks;
	t_token	*next;

	new_tks = tokenize1(gc, value);
	while (new_tks)
	{
		next = new_tks->next;
		new_tks->next = NULL;
		*cur = new_tks;
		cur = &new_tks->next;
		new_tks = next;
	}
	return (*cur);
}

static void	handle_expansion(t_gc *gc, t_token **cur, t_token *tk, t_env *env, t_cmd *cmd)
{
	char	*exp;
	t_token	*tmp;
	t_token	*last;

	exp = expand_value(gc, tk->value, env, cmd);
	tmp = tokenize1(gc, exp);
	last = *cur;
	while (tmp)
	{
		if (last)
			last->next = tmp;
		last = tmp;
		tmp = tmp->next;
	}
	*cur = last;
}

int contains_special_char(const char *str)
{
    if (!str)
        return 0;

    while (*str)
    {
        if (*str == '"' || *str == '\\' /* || أضف رموز أخرى */)
            return 1;
        str++;
    }
    return 0;
}

char *handle_special_chars(t_gc *gc, char *value)
{
	t_dynstr ds;
	const char *src = value;
	int s = 0, d = 0;

	dyn_str_init(&ds);
	while (*src)
	{
		if ((*src == '\'' && !d) || (*src == '"' && !s))
		{
			handle_quotes(gc, *src++, &s, &d, &ds);
		}
		else if (*src == '\\' && !s)
		{
			src++;
			if (*src)
				dyn_str_append(gc, &ds, src++, 1);
		}
		else
		{
			char tmp[1] = {*src++};
			dyn_str_append(gc, &ds, tmp, 1);
		}
	}
	return dyn_str_finalize(gc, &ds);
}

t_token	*expand(t_gc *gc, t_token *tk, t_env *env, t_cmd *cmd)
{
	t_token	head;
	t_token	*cur;
	t_token	*next;

	head.next = NULL;
	cur = &head;
	while (tk)
	{
		next = tk->next;
		tk->next = NULL;
		if (contains_special_char(tk->value))
		{
			tk->value = handle_special_chars(gc, tk->value);
			cur->next = tk;
			cur = cur->next;
			tk = next;
			continue;
		}
		if (tk->type == TOKEN_WORD && ft_strchr(tk->value, '$'))
			handle_expansion(gc, &cur, tk, env, cmd);
		else
		{
			cur->next = tk;
			cur = cur->next;
		}
		tk = next;
	}
	return (head.next);
}




static char *append_to1_result1(char *result, char *str, size_t *total)
{
    size_t len = ft_strlen(str);
    char *new_result = malloc(*total + len + 1);
    if (!new_result)
        return NULL;

    if (result)
        ft_memcpy(new_result, result, *total);

    ft_memcpy(new_result + *total, str, len);
    *total += len;
    new_result[*total] = '\0';

    free(result); // مهم نحرر الذاكرة القديمة
    return new_result;
}


char *expand_herdoc(t_token *token, t_env *env)
{
    char *result = NULL;
    size_t total = 0;

    while (token) {
        char *src = token->value;

        while (*src) {
            if (*src == '$') {
                int dollar_count = 0;
                while (*src == '$') {
                    dollar_count++;
                    src++;
                }

                int escape_count = dollar_count / 2;
                int expand = dollar_count % 2;

                for (int i = 0; i < escape_count; i++) {
                    result = append_to1_result1(result, "$", &total);
                }

                if (!expand) continue;

                char varname[256] = {0};
                int brace = (*src == '{');
                int valid = 0;

                if (brace) {
                    src++;
                    int i = 0;
                    while (*src && *src != '}' && i < 255)
                        varname[i++] = *src++;
                    varname[i] = '\0';
                    if (*src == '}') src++;
                    valid = i > 0;
                } else if (isdigit(*src) || ft_isalpha(*src) || *src == '_') {
                    int i = 0;
                    while ((ft_isalnum(*src) || *src == '_') && i < 255)
                        varname[i++] = *src++;
                    varname[i] = '\0';
                    valid = i > 0;
                }

                if (!valid) {
                    result = append_to1_result1(result, "$", &total);
                    continue;
                }

                char *val = find_env(env, varname);
                if (!val) val = "";

                result = append_to1_result1(result, val, &total);
                continue;  // <=== مهم جداً !!!
            }

            char tmp[2] = {*src, '\0'};
            result = append_to1_result1(result, tmp, &total);
            src++;
        }

        if (token->next && token->next->type == TOKEN_SPACE) {
            result = append_to1_result1(result, " ", &total);
        }

        token = token->next;
    }
    // printf("expand_herdoc: %s\n", result ? result : "(null)");
    return result;
}
