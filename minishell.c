/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/10 16:09:50 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "include/minishell.h"

char *read_input(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
	{
		write(1, "minishell$ exit\n", 17);
		exit(0);
	}
	if (*input)
		add_history(input);
	return (input);
}


void sigint_handler(int sig)
{
    (void)sig;
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

void    f()
{
    system("leaks minishell");
}

int main(int ac, char **av, char **envp)
{
    // atexit(f);
    t_gc gc;
    (void)av;
    if (ac > 1)
        return (write(2, "minishell: too many arguments\n", 31) ,1);
    t_exp *head = NULL;
    t_exp **exp = &head;
    t_env *env = ft_create_env(envp, &env);
    *exp = ft_create_env_export(envp, exp);
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (1);
    cmd->ex_status = 0;
    while (1)
    {
        gc_init(&gc);
        t_heredoc *heredoc = gc_malloc(&gc, sizeof(t_heredoc));
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
        t_cmd *parsed_cmd = parse(&gc, env, heredoc, &cmd);
        if (!parsed_cmd)
        {
            gc_free_all(&gc);
            continue;
        }
        ft_execute(exp, &env, &parsed_cmd, envp, heredoc);
        cmd->ex_status = parsed_cmd->ex_status;
        dup2(STDERR_FILENO, STDOUT_FILENO), gc_free_all(&gc);
    }
    (free(cmd), ft_env_clear(&env), ft_exp_clear(exp));
    return (0);
}
