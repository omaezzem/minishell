/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/30 16:54:14 by omaezzem         ###   ########.fr       */
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
    // rl_replace_line("", 0);s
    write(1, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

int cmdline_argc;
char **cmdline_argv;
int cmdline_shift;
int prev_status;

int main(int ac, char **av, char **env)
{
    t_cmd   *data;

    data = NULL;
    cmdline_argc = ac;
    cmdline_argv = av;
    cmdline_shift = 0;
    prev_status = 0;
    (void)env;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    // rl_catch_signals = 0;

    while (1)
    {
        if (!(data = parse()))
            continue;
        printf("%s\n", data->cmd[0]);
        ft_execute(data, data->cmd, env);
    }
    return 0;
}