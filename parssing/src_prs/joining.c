/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:33:09 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/24 13:25:34 by omaezzem         ###   ########.fr       */
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

    for (int i = 0; i < len; i++)
        new_arr[i] = arr[i];

    new_arr[len] = ft_strdup(str);
    new_arr[len + 1] = NULL;

    free(arr);
    return new_arr;
}



void handle_redirection(t_token **cur, char ***redir, char ***files)
{
    t_token *str = *cur;

    *redir = append_string_array(*redir, str->value); // Add > or < etc.
    str = str->next;

    while (str && str->type == TOKEN_SPACE)
        str = str->next;

    if (str && str->type == TOKEN_FILE)
    {
        *files = append_string_array(*files, str->value);
        str = str->next;
    }
    else
    {
        *files = append_string_array(*files, ft_strdup(""));
    }

    *cur = str;
}

void split_cmd_args(char **full, char ***cmd, char ***args)
{
    int len = 0;
    while (full && full[len])
        len++;

    if (len == 0)
    {
        *cmd = NULL;
        *args = NULL;
        return;
    }

    *cmd = malloc(sizeof(char *) * 2);
    (*cmd)[0] = ft_strdup(full[0]);
    (*cmd)[1] = NULL;

    if (len == 1)
    {
        *args = NULL;
        return;
    }

    *args = malloc(sizeof(char *) * len);
    for (int i = 1; i < len; i++)
        (*args)[i - 1] = ft_strdup(full[i]);
    (*args)[len - 1] = NULL;
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
// void print_cmd_list(t_cmd *cmd)
// {
//     // int i;
//     // while (cmd)
//     // {
//     //     for (i = 0; cmd->cmd && cmd->cmd[i]; i++)
//     //         printf("cmd->cmd[%d] = %s\n", i, cmd->cmd[i]);

//     //     for (i = 0; cmd->redirection && cmd->redirection[i]; i++)
//     //         printf("cmd->redirection[%d] = %s\n", i, cmd->redirection[i]);

//     //     for (i = 0; cmd->files && cmd->files[i]; i++)
//     //         printf("cmd->files[%d] = %s\n", i, cmd->files[i]);

//     //     cmd = cmd->next;
//     // }
// }


t_cmd *joining2(t_token *tokens)
{
    t_cmd *head = NULL, *last = NULL;

    while (tokens)
    {
        t_cmd *node = malloc(sizeof(t_cmd));
        if (!node)
            return NULL;

        // Initialize
        node->cmd = NULL;
        node->args = NULL; // unused
        node->files = NULL;
        node->redirection = NULL;
        node->next = NULL;
        node->type = 0;
        node->env = NULL;
        node->value_expand = NULL;
        node->workdir = NULL;
        node->ex_status = 0;

        while (tokens && tokens->type != TOKEN_PIPE)
        {
            if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_OPTION || tokens->type == TOKEN_FILE)
            {
                node->cmd = append_string_array(node->cmd, tokens->value);
                tokens = tokens->next;
            }
            else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
                     tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
            {
                handle_redirection(&tokens, &node->redirection, &node->files);
            }
            else
            {
                tokens = tokens->next;
            }
        }

        if (!head)
            head = node;
        else
            last->next = node;
        last = node;

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    // print_cmd_list(head);
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
            if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_OPTION || tokens->type == TOKEN_FILE)
            {
                args = append_string_array(args, tokens->value);
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
