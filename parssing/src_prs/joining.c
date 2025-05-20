/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:33:09 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/20 15:01:13 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"
char **append_string_array(char **arr, char *str)
{
    int len = 0;
    while (arr && arr[len])
        len++;

    char **new_arr = malloc(sizeof(char *) * (len + 2));
    if (!new_arr)
        return NULL;
    // char *tmp = str;
    // while (tmp && *tmp)
    // {
    //     printf("Value: %s\n", tmp);
    //     tmp++;
    // }
    // int i = 0;
    // while
    for (int i = 0; i < len; i++)
    {
        // printf("Value-----: %s\n", arr[i]);
        new_arr[i] = arr[i];
    }
    new_arr[len] = ft_strdup(str);
    new_arr[len + 1] = NULL;
    // char **tmp = new_arr;
    // while (tmp && *tmp)
    // {
    //     printf("Value: %s\n", *tmp);
    //     tmp++;
    // }

    // printf("Appending: %s\n", str); // Only print what's being appended

    free(arr);
    return new_arr;
}



void handle_redirection(t_token **cur, char ***redir, char ***files)
{
    t_token *str = *cur;

    while (str && str->type == TOKEN_SPACE)
        str = str->next;

    if (str && (str->type == TOKEN_REDIRECT_OUT || str->type == TOKEN_REDIRECT_IN ||
                str->type == TOKEN_APPEND || str->type == TOKEN_HEREDOC))
    {
        *redir = append_string_array(*redir, str->value);
        str = str->next;

        while (str && str->type == TOKEN_SPACE)
            str = str->next;

        if (str && str->type == TOKEN_FILE) {
            *files = append_string_array(*files, str->value);
            str = str->next;
        } else {
            printf("Error: No filename found after redirection\n");
            *files = append_string_array(*files, ft_strdup(""));  // or NULL/placeholder
        }
    }

    *cur = str;  // move pointer forward
}


t_cmd *create_command_block_from_arrays(char **args, char **redir, char **files)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;

    cmd->cmd = args;
    cmd->redirection = redir;
    cmd->files = files;
    cmd->ex_status = 0;
    cmd->next = NULL;
    return cmd;
}

t_cmd *joining2(t_token *tokens)
{
    t_cmd *head = NULL, *last = NULL;

    char **args = NULL;
    while (tokens)
    {
        // char **redir = NULL;
        // char **files = NULL;

        while (tokens && tokens->type != TOKEN_PIPE)
        {
            if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_OPTION || tokens->type == TOKEN_FILE)
            {
                args = append_string_array(args, tokens->value);
                tokens = tokens->next;
            }
            else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
                     tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
            {
                // new t_cmd node per redirection
                char **redir_arr = NULL;
                char **file_arr = NULL;

                handle_redirection(&tokens, &redir_arr, &file_arr);

                t_cmd *node = create_command_block_from_arrays(
                    args ? ft_split(args[0], ' ') : NULL, // Only first word as base command
                    redir_arr,
                    file_arr
                );

                if (!head)
                    head = node;
                else
                    last->next = node;
                last = node;
            }
            else
            {
                tokens = tokens->next;
            }
                // printf("Value: %s\t", tokens->value);
        }

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
        //     int i;
        // int j = 0;
        // t_cmd *cmdd = head;
        // while (cmdd->cmd)
        // {
        //     i = 0;
        //     while (cmdd->cmd[i])
        //     {
        //         printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>: %s\n", j,  cmdd->cmd[i]);
        //         // printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>: %s\n", j,  cmdd->cmd[i]);
        //         i++;
        //     }
        //     cmdd = cmdd->next;
        //     j++;
        // }
    return head;
}
char **duplicate_string_array(char **arr)
{
    if (!arr)
        return NULL;
    int len = 0;
    while (arr[len])
        len++;

    char **copy = malloc(sizeof(char *) * (len + 1));
    if (!copy)
        return NULL;

    for (int i = 0; i < len; i++)
        copy[i] = ft_strdup(arr[i]);
    copy[len] = NULL;
    return copy;
}

void free_string_array(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

t_cmd *joining(t_token *tokens)
{
    t_cmd *head = NULL, *last = NULL;
    char **args = NULL; // accumulated args for current command
    int first_cmd_node_created = 0; // track if we assigned command for this command group

    while (tokens)
    {
        // Reset flag for new command group after pipe
        first_cmd_node_created = 0;

        while (tokens && tokens->type != TOKEN_PIPE)
        {
            if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_FILE)
            {
                args = append_string_array(args, tokens->value);
                // char **tmp = args;
                // while (tmp && *tmp)
                // {
                //     printf("Value: %s\n", *tmp);
                //     tmp++;
                // }
                tokens = tokens->next;
            }
            else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
                     tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
            {
                // Handle redirection - create a new node for each redirection
                char **redir_arr = NULL;
                char **file_arr = NULL;

                handle_redirection(&tokens, &redir_arr, &file_arr);

                // For the **first** redirection node in this command group, pass the command args
                // For subsequent redirections on the same command, cmd=NULL
                char **cmd_args_for_node = NULL;
                if (!first_cmd_node_created && args)
                {
                    cmd_args_for_node = duplicate_string_array(args);
                    first_cmd_node_created = 1;
                }

                t_cmd *node = create_command_block_from_arrays(
                    cmd_args_for_node,
                    redir_arr,
                    file_arr
                );

                if (!head)
                    head = node;
                else
                    last->next = node;
                last = node;

                // After creating a node with cmd args, free the args array so next redirection nodes have no cmd
                if (cmd_args_for_node)
                {
                    free_string_array(args);
                    args = NULL;
                }
            }
            else
            {
                tokens = tokens->next;
            }
        }

        // If pipe or end reached, and there are still args left (no redirection)
        if (args)
        {
            // Create a final node for the last command segment without redirection
            t_cmd *node = create_command_block_from_arrays(duplicate_string_array(args), NULL, NULL);

            if (!head)
                head = node;
            else
                last->next = node;

            last = node;
            free_string_array(args);
            args = NULL;
        }

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next; // advance past pipe token
    }

    return head;
}
