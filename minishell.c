/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/03 17:01:43 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "include/minishell.h"

char *read_input(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
	{
		write(1, "exit\n", 7);
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

int cmdline_argc;
char **cmdline_argv;
int cmdline_shift;
int prev_status;


void    f()
{
    system("leaks minishell");
}
//handli SHLVL
//
// void handle_heredoc(char *delimiter, int *fd) {
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t read;
//     int tmp_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
//     if (tmp_fd < 0) {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }

//     while (1) {
//         write(STDOUT_FILENO, "> ", 2);
//         read = getline(&line, &len, stdin);
//         if (read == -1) {
//             perror("getline");
//             break;
//         }
//         // Remove newline character
//         if (line[read - 1] == '\n')
//             line[read - 1] = '\0';
//         if (ft_strcmp(line, delimiter) == 0)
//             break;
//         write(tmp_fd, line, ft_strlen(line));
//         write(tmp_fd, "\n", 1);
//     }
//     free(line);
//     close(tmp_fd);

//     tmp_fd = open(".heredoc_tmp", O_RDONLY);
//     if (tmp_fd < 0) {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }
//     *fd = tmp_fd;
// }

int main(int ac, char **av, char **envp)
{

   // atexit(f);

    cmdline_argc = ac;
    cmdline_argv = av;
    cmdline_shift = 0;
    prev_status = 0;
    // int fd = 0;
    if (ac != 1)
    {
        write(2, "Error: No arguments expected\n", 30);
        return (1);
    }
    t_cmd *cmd = NULL;
    t_exp *head;
    t_env *env = ft_create_env(envp, &env);
    t_exp **exp;
    t_heredoc *heredoc = malloc(sizeof(t_heredoc));
    if (!heredoc)
    {
        perror("malloc");
        return (1);
    }
    heredoc->delimiter = NULL;
    heredoc->flag_heredoc = 0;
    head = NULL;
    exp = &head;
    *exp = ft_create_env_export(envp, exp);

    while (1)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
        cmd = parse(env, heredoc, cmd);
        if (!cmd)
            continue;
        ft_execute(exp, &env, cmd, envp, heredoc);
        dup2(STDERR_FILENO,STDOUT_FILENO);
    }
    return 0;
}
