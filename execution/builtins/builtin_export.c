/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:23:25 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/26 18:01:39 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	search_var_in_env(t_env *env, char *var)
{
	t_env *tmp;

	tmp = env;
	while (tmp->next != NULL)
	{
		if (ft_strcmp(tmp->var, var) == 0)
			return 1;
		tmp = tmp->next;
	}
	return 0;
}

int	search_var_in_exp(t_exp *exp, char *var)
{
	t_exp *tmp;

	tmp = exp;
	while (tmp->next != NULL)
	{
		if (ft_strcmp(tmp->vr, var) == 0)
			return 1;
		tmp = tmp->next;
	}
	return 0;
}
int	parse_input(char *args)
{
	int i;

	i = 0;
    while (args[i])
    {
        if(!ft_isalnum(args[i]))
            return (0);
        i++;
    }
    return (1);
}

int	find_equal(char *str)
{
	int i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '=')
	{
		printf("minishell : export: not a valid identifier\n");
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
		eqsplit = ft_split(env[i], '=');
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

void    add_to_export_list(t_exp **exp, char *args)
{
    char **splitargs;
    t_exp   *add;
    char    *strch;
    int     strl;
	int		a;
    char    *join;

    splitargs = mysplit(args, '=');
    if (len_equal(args) == 1)
    {
        if (search_var_in_exp(*exp, splitargs[0]))
        	update_val_exp(*exp, splitargs[0],splitargs[1]);
        add = malloc(sizeof(t_exp));
        if (!add)
        {
            free_split(splitargs);
            return ;
        }
        add->vr = ft_strdup(splitargs[0]);
        if (splitargs[1])
            add->vl = ft_strdup(splitargs[1]);
        else if (!splitargs[1])
            add->vl = ft_strdup("");
        ft_lstadd_back_exp(exp, add);
    }
    if (len_equal(args) > 1)
    {
        strch = ft_strchr_add_one(args, '=');
        strl = ft_strlen(strch);
        join = malloc(strl + 1);
        a = 0;
        while (strch[a])
        {
            join[a] = strch[a];
            a++;
        }
        join[a] = '\0';
        if (search_var_in_exp(*exp, splitargs[0]))
            update_val_exp(*exp, splitargs[0], add->vl);
        add = malloc(sizeof(t_exp));
        if (!add)
            return ;
        add->vr = ft_strdup(splitargs[0]);
        add->vl = ft_strdup(join);
        ft_lstadd_back_exp(exp, add);
    }
    
}

int ft_export(t_env *env, char **args, char **envp)
{
	int		len;
	t_exp	*curexp;
	int		i;
    int     strl;
    int     a;
	char	*add;
    char    **varval;
    char    *strch;
    t_env   *nnode;
    t_exp   **exp;

	// exp = NULL;
    exp = ft_create_env_export(envp, exp);
	printf("%s\n", (*exp)->vr = (*exp)->vl);
	len = len_arg(args);
	if (len > 1)
	{	
		if (ft_isnum(args[1][0]))
		{
			printf("export: `%s': not a valid identifier\n", args[1]);
			return (0);
		}
		i = 1;
		while (args[i])
		{
			if (!parse_input(args[i]))
				printf("export: `%s': not a valid identifier", args[i]);
			else 
				add_to_export_list(exp, args[i]);
			i++;
		}
	}
	curexp = *exp;
	if (len == 1 && (ft_strcmp(args[0], "./export") == 0))
	{
		// printf("%s\n", args[0]);
		while (curexp != NULL)
		{
			printf("declare -x ");
            printf("%s", curexp->vr);
            printf("\"%s\n\"", curexp->vl);
			curexp = curexp->next;
		}
	}
	i = 1;
	while (args[i])
	{
		varval = mysplit(args[i], '=');
        if (!varval)
		return 0;
        if ((find_equal(args[i]) == 1) && (len_equal(args[i]) == 1))
		{
			if (search_var_in_env(env, varval[0]))
				update_val_env(env, varval[0], varval[1]);
            if (!varval[1])
                varval[1] = ft_strdup("\"\"");
            nnode = malloc(sizeof(t_env));
            if (!nnode)
            {
                free_split(varval);
                return (0);
            }
            nnode->var = ft_strdup(varval[0]);
            nnode->val = ft_strdup(varval[1]);
            ft_lstadd_back_env(&env, nnode);
		}
        else if(find_equal(args[i]) == 1 && (len_equal(args[i]) > 1))
        {
            strch = ft_strchr_add_one(args[i], '=');
            strl = ft_strlen(strch);
            add = malloc(strl + 1);
            a = 0;
            while (strch[a])
            {
                add[a] = strch[a];
                a++;
            }
            add[a] = '\0';
			if (search_var_in_env(env, varval[0]))
				update_val_env(env, varval[0], add);
            nnode = malloc(sizeof(t_env));
            if (!nnode)
                return 0;
            nnode->var = ft_strdup(varval[0]);
            nnode->val = ft_strdup(add);
            ft_lstadd_back_env(&env, nnode);
        }
	}
	return (0);
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