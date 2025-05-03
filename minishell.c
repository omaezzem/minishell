/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/03 16:14:36 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

char *read_input(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
	{
		write(1, "exit\n", 6);
		exit(0);
	}
	if (*input)
		add_history(input);
	return (input);
}


void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

int cmdline_argc;
char **cmdline_argv;
int cmdline_shift;
int prev_status;

int main(int ac, char **av, char **envp)
{
    cmdline_argc = ac;
    cmdline_argv = av;
    cmdline_shift = 0;
    prev_status = 0;
    t_cmd *cmd = NULL;
    t_exp *head;
    t_env *env = ft_create_env(envp, &env);
    t_exp **exp;
    head = NULL;
    exp = &head;
    exp = ft_create_env_export(envp, exp);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        cmd = parse(env);
        if (!cmd)
            continue;
        ft_execute(*exp, env, cmd, cmd->cmd);
    }
    return 0;
}