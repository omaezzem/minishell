/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:17:48 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/20 15:28:06 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

# define FAILURE 0
# define SUCCESS 1

typedef struct s_env
{
    char            *var;
    char            *val;
    struct  s_env   *next;
}       t_env;
 
/*---------------------------------------------utils---------------------------------------------*/

t_env	*ft_lstnew_val(char *val);
t_env	*ft_lstnew_var(char *var);
void	ft_lstadd_back(t_env **lst, t_env *new);
int		ft_strcmp(char *s1, char *s2);
char	**ft_split(char *s, char c);
void	free_split(char **arr);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *s);

/*---------------------------------------------builtin---------------------------------------------*/
void	ft_echo(char **args);
void ft_create_env(char **env, t_env **ev);




#endif