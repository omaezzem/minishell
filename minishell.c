/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/01 17:52:51 by omaezzem         ###   ########.fr       */
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

int main(int ac, char **av, char **envp)
{
    // t_env *env_list = NULL;

    // t_cmd *cmd;
    cmdline_argc = ac;
    cmdline_argv = av;
    cmdline_shift = 0;
    prev_status = 0;
    // (void)envp;
    t_cmd *cmd = NULL;
    // t_env *env_list = NULL;
    t_env *env = ft_create_env(envp, &env);
    
    // while (env)
    // {
    //     printf("var: %s\n", env->var);
    //     printf("val: %s\n", env->val);
    //     env = env->next;
    // }


    // Create env list from envp
        // t_env *env_list = getenv(envp[0]);
        // printf("var1: %s\n", env_list->var);
        // printf("env_buffer1: %s\n", env_list->val);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    // rl_catch_signals = 0;
    // init(cmd, envp);
    while (1)
    {
        cmd = parse();
        if (!cmd)
            continue;
    //     // if (!parse())
    //         // continue;
    // if (cmd->env)
    // {
    //     printf("cmd[0] = %s\n", cmd->cmd[0]);
    //     printf("cmd->env->val = %s\n", cmd->env->val); // Accessing the value of the environment variable
    // }
        ft_execute(cmd, cmd->cmd, envp);
    }

    return 0;
}