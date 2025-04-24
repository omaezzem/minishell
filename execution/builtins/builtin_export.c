#include "../../include/minishell.h"

int	find_equal(char *str)
{
	int i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '=')
	{
		return ;
		printf("minishell : export: `=2': not a valid identifier\n");
	}
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
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


int ft_export(t_cmd *data, t_env *env, char **args)
{
	int		len;
	t_env	*curr;
	int		i;
	int		j;
    int     strl;
    int     a;
	char	*add;
    char    **varval;
    char    *strch;
    t_env   *nnode;
	int		k;

	curr = env;
	len = len_arg(args);
	if (len == 1 && (ft_strcmp(args[0], "export") == 0))
	{
		while (curr->next != NULL)
		{
			printf("declare -x ");
            printf("\"%s = %s\n\"", curr->var, curr->val);
			curr = curr->next;
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
            if (!varval[1])
                varval[1] = ft_strdup("\"\"");
            nnode = malloc(sizeof(t_env));
            if (!nnode)
            {
                free_split(varval);
                return NULL;
            }
            nnode->var = ft_strdup(varval[0]);
            nnode->val = ft_strdup(varval[1]);
            ft_lstadd_back(&env, ft_lstnew_var(nnode));
		}
        else if(find_equal(args[i] == 1) && (len_equal(args[i] > 1)))
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
            strch[a] = '\0';
            nnode = malloc(sizeof(t_env));
            if (!nnode)
            {
                free_split(add);
                return NULL;
            }
            nnode->var = ft_strdup(varval[0]);
            nnode->val = ft_strdup(add);
            ft_lstadd_back(&env, ft_lstnew_var(nnode));
        }
	}
}
