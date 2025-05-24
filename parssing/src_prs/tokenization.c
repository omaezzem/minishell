/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:14:49 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/22 16:40:52 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_type get_token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return TOKEN_PIPE;
	if (ft_strcmp(str, ">>") == 0)
		return TOKEN_APPEND;
	if (ft_strcmp(str, "||") == 0)
		return TOKEN_OR;
	if (ft_strcmp(str, "<<") == 0)
		return TOKEN_HEREDOC;
	if (ft_strcmp(str, "<") == 0)
		return TOKEN_REDIRECT_IN;
    // if (ft_strcmp(str, "'") == 0)
    //     return TOKEN_SQUOTE;
	if (ft_strcmp(str, ">") == 0)
		return TOKEN_REDIRECT_OUT;
	if (ft_strcmp(str, "(") == 0)
		return TOKEN_OPEN_PAREN;
	if (ft_strcmp(str, " ") == 0)
		return TOKEN_SPACE;
	if (ft_strcmp(str, ")") == 0)
		return TOKEN_CLOSE_PAREN;
	// if (str[0] == '-' || str[0] == '~')
	// 	return TOKEN_OPTION;
	if (ft_strcmp(str, "~/") == 0)
		return TOKEN_OPTION;
	// if (ft_strcmp(str,  "\"\"") == 0)
	// 	return TOKEN_DQUOTE;
    // if (ft_strchr(str, '\''))
    //     return TOKEN_WORD;
    // if (ft_strchr(str, \"\"'))
    //     return TOKEN_WORD;
	return TOKEN_WORD;
}

void print_type(t_type type)
{
	if (type == TOKEN_WORD)
		printf("TOKEN_WORD\n");
	else if (type == TOKEN_PIPE)
		printf("TOKEN_PIPE\n");
	else if (type == TOKEN_OR)
		printf("TOKEN_OR\n");
	else if (type == TOKEN_REDIRECT_IN)
		printf("TOKEN_REDIRECT_IN\n");
	else if (type == TOKEN_REDIRECT_OUT)
		printf("TOKEN_REDIRECT_OUT\n");
	else if (type == TOKEN_APPEND)
		printf("TOKEN_APPEND\n");
	else if (type == TOKEN_HEREDOC)
		printf("TOKEN_HEREDOC\n");
	else if (type == TOKEN_DQUOTE)
		printf("TOKEN_DQUOTE\n");
	else if (type == TOKEN_OPEN_PAREN)
		printf("TOKEN_OPEN_PAREN\n");
	else if (type == TOKEN_CLOSE_PAREN)
		printf("TOKEN_CLOSE_PAREN\n");
    else if (type == TOKEN_SQUOTE)
        printf("TOKEN_SQUOTE\n");
    else if (type == TOKEN_SPACE)
        printf("TOKEN_SPACE\n");
	// else if (type == TOKEN_OPTION)
	// 	printf("TOKEN_OPTION\n");
	else if (type == TOKEN_FILE)
		printf("TOKEN_FILE\n");
	else if (type == TOKEN_DQUOTE)
		printf("TOKEN_DQUOTE\n");
	// else
	// 	printf("UNKNOWN TOKEN\n");
}
int		issspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

void mark_file_tokens(t_token *tokens)
{
	while (tokens && tokens->next)
	{
		if (tokens->type == TOKEN_REDIRECT_IN ||
			tokens->type == TOKEN_REDIRECT_OUT ||
			tokens->type == TOKEN_APPEND ||
			tokens->type == TOKEN_HEREDOC)
		{
			if (tokens->next->type == TOKEN_SPACE &&
				tokens->next->next &&
				tokens->next->next->type == TOKEN_WORD)
			{
				tokens->next->next->type = TOKEN_FILE;
			}
			else if (tokens->next->type == TOKEN_WORD)
			{
				tokens->next->type = TOKEN_FILE;
			}
		}
		tokens = tokens->next;
	}
}

void hansle_space(int start, int *i, char *input, t_token **tokens)
{
    start = *i;
    while (issspace(input[*i]))
        (*i)++;
    char *space = ft_substr(input, start, *i - start);
    append_token(tokens, create_token(space, TOKEN_SPACE));
    free(space);
}
void handle_quotes(int *i, char *input, t_token **tokens)
{
    char quote = input[*i];
    int start = *i;
    (*i)++;  // Skip opening quote

    while (input[*i] && input[*i] != quote)
        (*i)++;

    if (input[*i] == quote)
        (*i)++;  // Include closing quote

    char *quoted = ft_substr(input, start, *i - start);
    append_token(tokens, create_token(quoted, TOKEN_WORD));
    free(quoted);
}

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        if (issspace(input[i]))
        {
            hansle_space(i, &i, input, &tokens);
            continue;
        }
        if (input[i] == '\'' || input[i] == '"')
        {
            handle_quotes(&i, input, &tokens);
            continue;
        }

        // Handle operators (existing code)
        if (ft_strchr("><|&", input[i]) && input[i] == input[i + 1]) {
            char op[3] = {input[i], input[i + 1], '\0'};
            append_token(&tokens, create_token(op, get_token_type(op)));
            i += 2;
            continue;
        }

        // Handle single-character operators
        if (ft_strchr("><|()", input[i])) {
            char op[2] = {input[i], '\0'};
            append_token(&tokens, create_token(op, get_token_type(op)));
            i++;
            continue;
        }

        // Handle variable assignments with quoted values
        int start = i;
        while (input[i] && !issspace(input[i]))
        {
            if (input[i] == '=' && (input[i+1] == '"' || input[i+1] == '\'')) {
                // Skip = and quote
                i += 2;
                char quote = input[i-1];

                // Find closing quote
                while (input[i] && input[i] != quote)
                    i++;

                if (input[i] == quote)
                    i++;  // Skip closing quote
            }
            else if (ft_strchr("><|()'\"", input[i])) {
                break;
            }
            else {
                i++;
            }
        }

        if (i > start) {
            char *word = ft_substr(input, start, i - start);
            append_token(&tokens, create_token(word, TOKEN_WORD));
            free(word);
        }
    }

    mark_file_tokens(tokens);
    return tokens;
}
t_token *tokenize_2(char *input)
{
    t_token *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        if (issspace(input[i])) {
            int start = i;
            while (issspace(input[i]))
                i++;
            char *space = ft_substr(input, start, i - start);
            append_token(&tokens, create_token(space, TOKEN_SPACE));
            free(space);
            continue;
        }
        if (ft_strchr("><|& ~/", input[i]) && input[i] == input[i + 1])
        {
            char op[3] = {input[i], input[i + 1], '\0'};
            append_token(&tokens, create_token(op, get_token_type(op)));
            i += 2;
            continue;
        }
        // Handle quotes (single and double quotes as separate tokens)
        if (input[i])
        {
            // char quote = input[i];  // Determine whether it's single or double quote
            int start = i;
            i++;  // Skip the opening quote

            // Capture everything inside the quotes until we find the closing quote
            while (input[i] && input[i] != ' ')
                i++;
            // If we reach the end of the string and no closing quote, print error
            // if (input[i] == '\0') {
            //     printf("minishell: syntax error: unmatched quote '%c'\n", quote);
            //     return NULL;  // Return NULL indicating an error
            // }

            // Otherwise, we found the closing quote, so move past it
            i++;  // Skip the closing quote

            // Optionally, you can add the quoted string as a token here
            char *quoted = ft_substr(input, start, i - start);  // Extract the quoted string)
            append_token(&tokens, create_token(quoted, get_token_type(quoted)));
            free(quoted);  // Free the extracted quoted string
            continue;
        }
        if (input[i] == '$' || input[i + 1] != '{')
        {
        if (input[i] == '$') {
            int start = i++;
            if (input[i] == '{') {
                i++; // skip '{'
                while (input[i] && input[i] != '}')
                    i++;
                if (input[i] == '\0') {
                    printf("minishell: syntax error: unmatched '{'\n");
                    return NULL;
                }
                i++; // skip '}'
            } else {
                while (isalnum(input[i]) || input[i] == '_')
                    i++;
            }
            char *var = ft_substr(input, start, i - start);
            append_token(&tokens, create_token(var, get_token_type(var)));
            free(var);
            continue;
            }

        }
        // Handle double-character operators (e.g., >>, ||, etc.)
        // Handle single-character operators (e.g., <, >, |, (, ))
        if (ft_strchr("><|()'", input[i]))
        {
            char op[2] = {input[i], '\0'};
            append_token(&tokens, create_token(op, get_token_type(op)));
            i++;
            continue;
        }

        // Handle regular words (non-special tokens)
        int start = i;
        while (input[i] && !issspace(input[i]) && !ft_strchr("><|()'\"'", input[i]))
            i++;
        char *word = ft_substr(input, start, i - start);
        append_token(&tokens, create_token(word, get_token_type(word)));
        free(word);
    }

    mark_file_tokens(tokens);  // Mark tokens related to file operations (if any)
    return tokens;
}

// t_cmd *parse(void)
// {
// 	t_token *tokens;
// 	t_cmd *cmd = NULL;
// 	char	*input;

// 	input = read_input("minishell$ ");
// 	tokens = NULL;
// 	tokens = tokenize(input);
// 	if (!tokens)
// 	{
// 		free(input);
// 		return NULL;
// 	}
// 	mark_file_tokens(tokens);
// 	free(input);
// 	t_token *tmp = tokens;
// 	while(tmp)
// 	{
// 		printf("Value: %s\t ", tmp->value);
// 		print_type(tmp->type);
// 		tmp = tmp->next;
// 	}
// 	if (tokens->type == TOKEN_PIPE)
// 	{
// 		printf("minishell: syntax error near unexpected token '%s'\n", tokens->value);
// 		return NULL;
// 	}
// 	if (!error(tokens))
// 		return NULL;
// 	if (!error_pipe(tokens))
// 		return NULL;
// 	cmd = joining(tokens);
// 	if (!expand(tokens))
// 	{
// 		printf("Expansion error\n");
// 		return NULL;
// 	}
// 	// free_token_list(tokens);
// 	return cmd;
// }
#include <string.h>
char *strip_quotes(char *str)
{
    size_t len = ft_strlen(str);
    char *result = malloc(len + 1);
    size_t i = 0, j = 0;
    if (!result)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            char quote = str[i++];
            while (str[i] && str[i] != quote)
                result[j++] = str[i++];
            if (str[i] == quote)
                i++;
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
    return result;
}

void remove_quotes_cmd(t_cmd *cmd)
{
    while (cmd)
    {
        // Remove quotes from cmd array
        if (cmd->cmd)
        {
            for (int i = 0; cmd->cmd[i]; ++i)
            {
                char *stripped = strip_quotes(cmd->cmd[i]);
                free(cmd->cmd[i]);
                cmd->cmd[i] = stripped;
            }
        }

        // Remove quotes from args array
        if (cmd->args)
        {
            for (int i = 0; cmd->args[i]; ++i)
            {
                char *stripped = strip_quotes(cmd->args[i]);
                free(cmd->args[i]);
                cmd->args[i] = stripped;
            }
        }

        // Remove quotes from files array
        if (cmd->files)
        {
            for (int i = 0; cmd->files[i]; ++i)
            {
                char *stripped = strip_quotes(cmd->files[i]);
                free(cmd->files[i]);
                cmd->files[i] = stripped;
            }
        }

        // Remove quotes from redirection array
        if (cmd->redirection)
        {
            for (int i = 0; cmd->redirection[i]; ++i)
            {
                char *stripped = strip_quotes(cmd->redirection[i]);
                free(cmd->redirection[i]);
                cmd->redirection[i] = stripped;
            }
        }

        cmd = cmd->next;
    }
}

// Helper: append string to NULL-terminated array
// char **append_string_array(char **arr, const char *str)
// {
//     int len = 0;
//     while (arr && arr[len])
//         len++;

//     char **new_arr = malloc(sizeof(char *) * (len + 2));
//     if (!new_arr)
//         return NULL;

//     for (int i = 0; i < len; i++)
//         new_arr[i] = arr[i];

//     new_arr[len] = strdup(str);
//     new_arr[len + 1] = NULL;

//     if (arr)
//         free(arr);

//     return new_arr;
// }

// t_cmd *create_command2_block_from_arrays(char **args, char **redir, char **files)
// {
//     t_cmd *cmd = malloc(sizeof(t_cmd));
//     if (!cmd)
//         return NULL;

//     cmd->args = args;
//     cmd->redirection = redir;
//     cmd->files = files;
//     cmd->ex_status = 0;
//     cmd->next = NULL;
//     return cmd;
// }
// void handle2_redirection(t_token **cur, char **redir, char **files)
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
// void print_command2_blocks(t_cmd *head)
// // static void print_cmd_blocks_debug(t_cmd *head)
// {
//     int i = 0;
//     t_cmd *tmp = head;
//     while (tmp)
//     {
//         // Reconstruct the full command string
//         if (tmp->cmd)
//         {
//             printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", i++);
//             for (int j = 0; tmp->cmd[j]; j++)
//             {
//                 printf("%s", tmp->cmd[j]);
//                 if (tmp->cmd[j + 1])
//                     printf(" ");
//             }
//             printf("\n");
//         }
//         tmp = tmp->next;
//     }
// }
// t_cmd *joining_no_split_quotes(t_token *tokens)
// {
//     t_cmd *head = NULL, *last = NULL;

//     while (tokens)
//     {
//         char **args = NULL;
//         char **redir = NULL;
//         char **files = NULL;

//         while (tokens && tokens->type != TOKEN_PIPE)
//         {
//             if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_OPTION || tokens->type == TOKEN_FILE)
//             {
//                 args = append_string_array(args, tokens->value);
//             }
//             else if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN ||
//                      tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
//             {
//                 handle2_redirection(&tokens, (char **)&redir, (char **)&files);
//                 continue;  // don't advance tokens here because handle_redirection updates it internally
//             }
//             tokens = tokens->next;
//         }

//         t_cmd *node = create_command2_block_from_arrays(args, redir, files);
//         if (!head)
//             head = node;
//         else
//             last->next = node;
//         last = node;

//         if (tokens && tokens->type == TOKEN_PIPE)
//             tokens = tokens->next;  // skip pipe token
//     }

//     print_command2_blocks(head);
//                 printf("joining_no_split_quotes\n");
//     return head;
// }

int check_ambiguous_redirection(t_token *tokens, t_env *env)
{
    t_token *cur = tokens;
    char *expanded = NULL;

    while (cur)
    {
        if (cur->type == TOKEN_REDIRECT_IN || cur->type == TOKEN_REDIRECT_OUT ||
            cur->type == TOKEN_APPEND || cur->type == TOKEN_HEREDOC)
        {
            // Skip space(s)
            t_token *target = cur->next;
            while (target && target->type == TOKEN_SPACE)
                target = target->next;

            if (target && target->type == TOKEN_FILE && target->value[0] == '$')
            {
                expanded = expand(target, env);
                if (!expanded || ft_strcmp(expanded, "") == 0)
                {
                    printf("minishell: %s: ambiguous redirect\n", target->value);
                    free(expanded);
                    return 0;
                }
                free(expanded);
            }
        }
        cur = cur->next;
    }
    return 1;
}



t_cmd *parse(t_env *env)
{
    t_token *tokens;
    t_cmd *cmd = NULL;
    char *input;
    char *quoted;

    input = read_input("minishell$ ");
    tokens = tokenize(input);
    free(input);
    if (!tokens)
        return NULL;

    mark_file_tokens(tokens);

    // Debug print: print all tokens and their types
    // t_token *tmp = tokens;
    // while (tmp)
    // {
    //     printf("Value: %s\t", tmp->value);
    //     print_type(tmp->type);
    //     tmp = tmp->next;
    // }

    // Basic syntax error checks
    if (tokens->type == TOKEN_PIPE)
    {
        printf("minishell: syntax error near unexpected token '%s'\n", tokens->value);
        return NULL;
    }

    if (!error(tokens, env)) return NULL;
    if (!error_pipe(tokens)) return NULL;
    if(check_ambiguous_redirection(tokens, env) == 0)
        return NULL;

    // Expand environment variables
    quoted = expand(tokens, env);
    if (!quoted)
    {
        printf("Expansion error\n");
        return NULL;
    }

    // Re-tokenize the expanded string
    tokens = tokenize(quoted);
    free(quoted);
    if (!tokens)
        return NULL;

    mark_file_tokens(tokens);

    // Build the command structure
//    int i;
//         int j = 0;
//         // t_cmd *cmdd = cmd;
//         while (cmdd->cmd)
//         {
//             i = 0;
//             while (cmdd->cmd[i])
//             {
//                 printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>: %s\n", j,  cmdd->cmd[i]);
//                 // printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>: %s\n", j,  cmdd->cmd[i]);
//                 i++;
//             }
//             cmdd = cmdd->next;
//             j++;
//         }
   cmd = joining2(tokens);
   if (!cmd)
    cmd = joining(tokens);
        // int i;
        // int j = 0;
        // t_cmd *cmdd = cmd;
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
    // t_token *tmp2 = tokens;
    // while (tmp2)
    // {
    //     printf("Value: %s\t", tmp2->value);
    //     print_type(tmp2->type);
    //     tmp2 = tmp2->next;
    // }
    // if (!cmd) {
        // if (!cmd)
        // {
    //         printf("Joining error\n");
    //         return NULL;
    //     }
    // }
        // if (!cmd)
        // {
        //     printf("Joining error\n");
        //     return NULL;
        // }
        // int i = 0;
        // t_cmd *cmdd = cmd;
        // while (cmdd)
        // {
        //     if (ft_strcmp(cmdd->redirection[i], ">>") == 0)
        //     {
        //         ft_output_append(cmdd->files[i], cmdd->cmd[0], i);
        //     }
        //     i++;
        // }
    // printf("cmd is NULL\n");
    // return NULL;
// }

    // while (cmd->cmd)
    // {
    //     printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>dgdfg>>>>>>>", i++);
    //     for (int j = 0; cmd->cmd[j]; j++)
    //     {
    //         printf("%s", cmd->cmd[j]);
    //         if (cmd->cmd[j + 1])
    //             printf(" ");
    //     }
    //     printf("\n");
    // }
    // Remove quotes from cmd fields (not from tokens)
    remove_quotes_cmd(cmd);
    // printf("cmd : %p\n", cmd->cmd[0]);
        //     int i;
        // int j = 0;
        // while (cmd->cmd)
        // {
        //     i = 0;
        //     while (cmd->cmd[i])
        //     {
        //         printf("cmd[%d]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>: %s\n", j,  cmd->cmd[i]);
        //         i++;
        //     }
        //     cmd = cmd->next;
        //     j++;
        // }
    return cmd;
}
