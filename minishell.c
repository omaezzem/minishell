/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:17:45 by omaezzem          #+#    #+#             */
/*   Updated: 2025/05/29 16:20:29 by omaezzem         ###   ########.fr       */
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
    t_cmd *cmd = NULL;
    t_exp *head;
    t_env *env = ft_create_env(envp, &env);
    t_exp *exp;
    head = NULL;
    exp = head;
    exp = ft_create_env_export(envp, &exp);

    // check
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    // int i = 0;
    env->flagenv = malloc(sizeof(int));
    while (1)
    {
        cmd = parse(env);
        if (!cmd)
            continue;
        // while (cmd)
        // {
        //     for (i = 0; cmd->cmd && cmd->cmd[i]; i++)
        //         printf("cmd->cmd[%d] = %s\n", i, cmd->cmd[i]);
        //     for (i = 0; cmd->files && cmd->files[i]; i++)
        //         printf("cmd->cmd[%d] = %s\n", i, cmd->files[i]);
        //     cmd = cmd->next;
        // }
        // handle_heredoc(cmd->files[0], &fd)
        ft_execute(&exp, &env, cmd, envp);
        dup2(STDERR_FILENO,STDOUT_FILENO);
    }
    return 0;
}
// void add_token_string(char **target, char *value)
// {
//     char *tmp;

//     if (*target)
//     {
//         tmp = ft_strjoin(*target, value);
//         free(*target);
//         *target = tmp;
//     }
//     else
//         *target = ft_strdup(value);

//     tmp = ft_strjoin(*target, " ");
//     free(*target);
//     *target = tmp;
// }

// void handle_redirection(t_token **cur, char **redir, char **files)
// {
//     char *tmp;
//     t_token *str = *cur;

//     while (str && str->type == TOKEN_SPACE)
//         str = str->next;

//     // Store redirection operator
//     if (str && (str->type == TOKEN_REDIRECT_OUT || str->type == TOKEN_REDIRECT_IN ||
//                 str->type == TOKEN_APPEND || str->type == TOKEN_HEREDOC))
//     {
//         if (*redir)
//         {
//             tmp = ft_strjoin(*redir, " ");
//             char *tmp2 = ft_strjoin(tmp, str->value);
//             free(tmp);
//             free(*redir);
//             *redir = tmp2;
//         }
//         else
//         {
//             *redir = ft_strdup(str->value);
//         }
//         str = str->next;
//     }

//     while (str && str->type == TOKEN_SPACE)
//         str = str->next;

//     // Store corresponding filename
//     if (str && str->type == TOKEN_FILE)
//     {
//         if (*files)
//         {
//             tmp = ft_strjoin(*files, " ");
//             char *tmp2 = ft_strjoin(tmp, str->value);
//             free(tmp);
//             free(*files);
//             *files = tmp2;
//         }
//         else
//         {
//             *files = ft_strdup(str->value);
//         }
//         str = str->next;
//     }
//     else
//     {
//         printf("Error: No filename found after redirection\n");
//     }

//     *cur = str;  // update the token pointer
// }




// t_cmd *create_command_block(char *command, char *redir, char *files)
// {
//     t_cmd *cmd = malloc(sizeof(t_cmd));
//     if (!cmd)
//         return NULL; // Ensure memory is allocated

//     // Split the command into arguments
//     cmd->cmd = ft_split(command, ' ');

//     // Split the redirection into parts
//     cmd->redirection = redir ? ft_split(redir, ' ') : NULL;

//     // Split the name file if any
//     cmd->files = ft_split(files, ' ');
//     // cmd->files = ft_split(files, ' ');
//     // Initialize other fields
//     cmd->ex_status = 0;
//     cmd->next = NULL;

//     return cmd;
// }

// void print_command_blocks(t_cmd *head)
// {
//     int i = 1;
//     t_cmd *node = head;

//     while (node)
//     {
//         // Print command
//         if (node->cmd)
//         {
//             printf("command     : ");
//             for (int j = 0; node->cmd[j]; j++)
//                 printf("%s ", node->cmd[j]);
//             printf("\n");
//         }

//         // Print redirections and corresponding files
//         int j = 0;
//         while (node->redirection && node->files &&
//                node->redirection[j] && node->files[j])
//         {
//             printf("redirection[%d] : %s\n", j, node->redirection[j]);
//             printf("file       [%d] : %s\n", j, node->files[j]);
//             j++;
//         }

//         node = node->next;
//         i++;
//     }
// }


// t_cmd *joining(t_token *tokens)
// {
//     t_cmd *head = NULL, *last = NULL;

//     while (tokens)
//     {
//         char *cmd = NULL, *opt = NULL, *redir = NULL, *files = NULL;

//         while (tokens && tokens->type != TOKEN_PIPE)
//         {
//             if (tokens->type == TOKEN_WORD)
//                 add_token_string(&cmd, tokens->value);
//             else if (tokens->type == TOKEN_OPTION)
//                 add_token_string(&opt, tokens->value);
//             else if (tokens->type == TOKEN_FILE)
//                 add_token_string(&opt, tokens->value);
//             else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
//                      tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
//             {
//                 handle_redirection(&tokens, &redir, &files);  // Do NOT advance tokens after this
//                 continue;  // Skip the default tokens = tokens->next below
//             }

//             tokens = tokens->next;  // Only advance when not inside handle_redirection
//         }

//         t_cmd *node = create_command_block(cmd, redir, files);
//         free(cmd); free(opt); free(redir); free(files);

//         if (!head)
//             head = node;
//         else
//             last->next = node;

//         last = node;

//         if (tokens && tokens->type == TOKEN_PIPE)
//             tokens = tokens->next;  // Skip the pipe token
//     }

//     print_command_blocks(head);
//     return head;
// }
