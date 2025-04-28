/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:23:25 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/28 12:57:28 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int search_var_in_env(t_env *env, char *var)
{
    if (!env || !var)
		return 0;
    t_env *tmp = env;
    while (tmp != NULL)
    {
        if (tmp->var && ft_strcmp(tmp->var, var) == 0)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

int	search_var_in_exp(t_exp *exp, char *var)
{
	t_exp *tmp;

	tmp = exp;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->vr, var) == 0)
			return 1;
		tmp = tmp->next;
	}
	return 0;
}
int	len_alnum_var(char *var)
{
	int i;
	int n;

	i = 0;
	n = 0;
	while(var[i])
	{
		if (ft_isalnum(var[i]))
			n++;
		i++;
	}
	return n;
}
char	*check_var(char *var)
{
	int i;
	int	lenvar;
	char *parsvar;

	lenvar = len_alnum_var(var);
	parsvar = malloc(sizeof(char) * lenvar + 1);
	if (!parsvar)
		return NULL;
	if (ft_isnum(var[0]))
	{
		printf("export: `%s': not a valid identifier\n", var);
		return NULL;
	}
	i = 0;
	while (var[i])
	{
		if (var[i] == '+' &&  var[i+1] != '\0')
		{
			printf("export: `%s': not a valid identifier\n", var);
			return NULL;
		}
		if (!ft_isalnum(var[i]))
		{
			if (var[i] == '+' &&  var[i+1] == '\0')
				break ;
			printf("export: `%s': not a valid identifier\n", var);
			free(parsvar);
			return NULL;
		}
		parsvar[i] = var[i];
		i++;
	}
	parsvar[i] = '\0';
	return parsvar;
}

int	find_equal(char *str)
{
	int i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '=')
	{
		printf("minishell : export '%s': not a valid identifier\n", str);
		return 0;
	}
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int len_at_first_equal(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break;
		i++;
	}
	return i;
}

int	len_equal(char *str)
{
	int i;
	int n;

	n = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			n++;
		i++;
	}
	return n;
}

t_exp **ft_create_env_export(char **env, t_exp **list)
{
	int		i;
	char	**eqsplit;
	t_exp	*new_node;
	
	i = -1;
	if (!env)
		return NULL;
	while (env[++i])
	{
		eqsplit = mysplit(env[i], '=');
		if (!eqsplit || !eqsplit[0] || !eqsplit[1])
		{
			free_split(eqsplit);
			continue;
		}
		new_node = malloc(sizeof(t_exp));
		if (!new_node)
		{
			free_split(eqsplit);
			return NULL;
		}
		new_node->vr = ft_strdup(eqsplit[0]);
		new_node->vl = ft_strdup(eqsplit[1]);
		new_node->next = NULL;
		ft_lstadd_back_exp(list, new_node);
		free_split(eqsplit);
	}
	return list;
}
void if_double_var(t_exp **exp)
{
    t_exp *current;
    t_exp *compare;
    t_exp *prev;

    if (!exp || !*exp)
		return;
    current = *exp;
    while (current != NULL && current->vr != NULL)
    {
        prev = current;
        compare = current->next;
        
        while (compare != NULL && compare->vr != NULL)
        {
            if (ft_strcmp(current->vr, compare->vr) == 0)
            {
                prev->next = compare->next;
                free(compare->vr);
                free(compare->vl);
                free(compare);
                compare = prev->next;
            }
            else
            {
                prev = compare;
                compare = compare->next;
            }
        }
        current = current->next;
    }
}
void	update_val_env(t_env *env, char *var, char *val)
{
	t_env	*add;

	add = env;
	while (env->next != NULL)
	{
		if (ft_strcmp(env->var, var) == 0)
		{
			free(env->val); 
			env->val = ft_strdup(val);
			env = add;
			break ;
		}
		env = env->next;
	}
	env = add;
}
void	update_val_exp(t_exp *exp, char *var, char *val)
{
	t_exp	*add = exp;

	while (exp->next != NULL)
	{
		if (ft_strcmp(exp->vr, var) == 0)
		{
			free(exp->vl);
			exp->vl = ft_strdup(val);
			exp = add;
			break ;
		}
		exp = exp->next;
	}
	exp = add;
}

void	add_to_export_list_vl(t_exp **exp, char *avzero, char *avone)
{
    t_exp   *add;

    if (search_var_in_exp(*exp, avzero))
		update_val_exp(*exp, avzero, avone);
	add = malloc(sizeof(t_exp));
	if (!add)
		return ;
	add->vr = ft_strdup(avzero);
	if (avone)
		add->vl = ft_strdup(avone);
	else if (!avone)
		add->vl = ft_strdup("");
	add->next = NULL;
	ft_lstadd_back_exp(exp, add);
}

void	add_to_export_list_v(t_exp **exp, char *avzero)
{
    t_exp   *add;

    if (search_var_in_exp(*exp, avzero))
		return ;
	add = malloc(sizeof(t_exp));
	if (!add)
		return ;
	add->vr = ft_strdup(avzero);
	add->vl = NULL;
	add->next = NULL;
	ft_lstadd_back_exp(exp, add);
}

char	*args_zero(char *args)
{
	int i;
	char	*zero;

	zero = malloc(len_at_first_equal(args) + 1);
	if (!zero)
		return NULL;
	i = 0;
	while (args[i])
	{
		if (args[i] == '=')
			break;
		zero[i] = args[i];
		i++;
	}
	zero[i] = '\0';
	return(zero);
}

char	*args_one(char *args)
{
	int i;

	i = 0;
	while (args[i])
	{
		if(args[i] == '=')
		{
			if (args[i+1] != '\0')
				return (&args[i+1]);
		}
		i++;
	}
	return NULL;
}
void	add_to_env_list(t_env *env, char *args, char *avzero, char *avone)
{
	t_env	*nnode;
	if (!env  || !args || !avzero || !avone) 
		return;

	if ((find_equal(args) == 1) && (len_equal(args) == 1))
	{
		if (search_var_in_env(env, avzero))
		{
			update_val_env(env, avzero, avone);
			return ;
		}
		nnode = malloc(sizeof(t_env));
		if (!nnode)
			return ;
		nnode->var = ft_strdup(avzero);
		nnode->val = ft_strdup(avone);
		nnode->next = NULL;
		ft_lstadd_back_env(&env, nnode);
	}
}
int ft_export(t_env *env, char **args, char **envp)
{
	int		len;
	t_exp	*curexp;
	int		i;
	t_exp	*head;
    t_exp   **exp;
	char	*avzero;
	char	*avone;

	head = NULL;
	exp = &head;
    exp = ft_create_env_export(envp, exp);
	len = len_arg(args);
    if (len > 1)
	{
        i = 1;
        while (args[i]) 
		{
            if (!args[i])
				continue;
            if (find_equal(args[i]))
			{
                avzero = check_var(args_zero(args[i]));
                avone = args_one(args[i]);
                if (!avone)
					avone = ft_strdup("");
                add_to_export_list_vl(exp, avzero, avone);
                add_to_env_list(env, args[i], avzero, avone);
            }
			else
			{
				avzero = check_var(args_zero(args[i]));
				if (avzero)
					add_to_export_list_v(exp, avzero);
			}
			i++;
		}
	}
	if_double_var(exp);
	curexp = *exp;
	while (curexp != NULL)
	{
		if (curexp->vr && !curexp->vl)
			printf("declare -x %s\n", curexp->vr);
		else
		{
			printf("declare -x ");
			printf("%s=", curexp->vr);
			if (curexp->vl)
				printf("\"%s\"\n", curexp->vl);
		}
		curexp = curexp->next;
	}
	return 0;
}

int main(int ac, char **av, char **env)
{
    t_env    *list;

	(void)ac;
    list = NULL;
    list = ft_create_env(env, &list);
	// printf("21\n");
    ft_export(list, av, env);
}
