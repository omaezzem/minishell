/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:14:49 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 23:42:28 by mel-badd         ###   ########.fr       */
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
	if (ft_strcmp(str, ">") == 0)
		return TOKEN_REDIRECT_OUT;
	if (ft_strcmp(str, "(") == 0)
		return TOKEN_OPEN_PAREN;
	if (ft_strcmp(str, " ") == 0)
		return TOKEN_SPACE;
	if (ft_strcmp(str, ")") == 0)
		return TOKEN_CLOSE_PAREN;
	if (ft_strcmp(str, "~/") == 0)
		return TOKEN_OPTION;
	return TOKEN_WORD;
}

// void print_type(t_type type)
// {
// 	if (type == TOKEN_WORD)
// 	else if (type == TOKEN_PIPE)
// 		printf("TOKEN_PIPE\n");
// 	else if (type == TOKEN_OR)
// 		printf("TOKEN_OR\n");
// 	else if (type == TOKEN_REDIRECT_IN)
// 		printf("TOKEN_REDIRECT_IN\n");
// 	else if (type == TOKEN_REDIRECT_OUT)
// 		printf("TOKEN_REDIRECT_OUT\n");
// 	else if (type == TOKEN_APPEND)
// 		printf("TOKEN_APPEND\n");
// 	else if (type == TOKEN_HEREDOC)
// 		printf("TOKEN_HEREDOC\n");
// 	else if (type == TOKEN_DQUOTE)
// 		printf("TOKEN_DQUOTE\n");
// 	else if (type == TOKEN_OPEN_PAREN)
// 		printf("TOKEN_OPEN_PAREN\n");
// 	else if (type == TOKEN_CLOSE_PAREN)
// 		printf("TOKEN_CLOSE_PAREN\n");
//     else if (type == TOKEN_SQUOTE)
//         printf("TOKEN_SQUOTE\n");
//     else if (type == TOKEN_SPACE)
//         printf("TOKEN_SPACE\n");
// 	// else if (type == TOKEN_OPTION)
// 	// 	printf("TOKEN_OPTION\n");
// 	else if (type == TOKEN_FILE)
// 		printf("TOKEN_FILE\n");
// 	else if (type == TOKEN_DQUOTE)
// 		printf("TOKEN_DQUOTE\n");
// 	// else
// 	// 	printf("UNKNOWN TOKEN\n");
// }
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
    while (tokens)
    {
        if (tokens->type == TOKEN_REDIRECT_IN ||
            tokens->type == TOKEN_REDIRECT_OUT ||
            tokens->type == TOKEN_APPEND ||
            tokens->type == TOKEN_HEREDOC)
        {
            t_token *next = tokens->next;
            while (next && next->type == TOKEN_SPACE)
                next = next->next;
            if (next && next->type == TOKEN_WORD)
            {
                printf("Marking token '%s' as TOKEN_FILE\n", next->value);
                next->type = TOKEN_FILE;
            }
        }
        tokens = tokens->next;
    }
}



int ft_issspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int redir_pipe(char c)
{
    return (c == '|' || c == '>' || c == '<' || c == '(' || c == ')');
}

int handle_redir(char *input, t_token **head, int i)
{
    int start = i;

    if ((input[i] == '>' || input[i] == '<') && input[i+1] == input[i])
        i += 2; // handles >> or <<
    else
        i++;

    char *tmp = ft_substr(input, start, i - start);
    t_token *new_token = create_token(tmp, get_token_type(tmp));
    append_token(head, new_token);
    free(tmp);
    return i;
}

int skip_quotes(const char *s, int i) {
    char quote = s[i++];
    while (s[i] && s[i] != quote) {
        if (s[i] == '\\' && quote == '\"' && s[i + 1])
            i += 2;
        else
            i++;
    }
    return (s[i] == quote) ? i + 1 : i; // skip closing quote if found
}

int add_word(char *input, t_token **head, int i)
{
    int start;

    start = i;
    while (input[i] && !ft_issspace(input[i]) && !redir_pipe(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i]) {
                printf("minishell: syntax error: unclosed quote\n");
                return -1;
            }
            i++;
        }
        else
            i++;
    }
    char *tmp = ft_substr(input, start, i - start);
    t_token *new_token = create_token(tmp, get_token_type(tmp));
    append_token(head, new_token);
    free(tmp);
    return i;
}
int add_word1(char *input, t_token **head, int i)
{
    int start;

    start = i;
    while (input[i] && !ft_issspace(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i]) {
                printf("minishell: syntax error: unclosed quote\n");
                return -1;
            }
            i++;
        }
        else
            i++;
    }
    char *tmp = ft_substr(input, start, i - start);
    t_token *new_token = create_token(tmp, TOKEN_WORD);
    append_token(head, new_token);
    free(tmp);
    return i;
}



t_token *tokenize(char *input)
{
    t_token *head = NULL;
    int i = 0;

    while (input[i] && ft_issspace(input[i]))
        i++; // Skip leading spaces
    while (input[i])
    {
        if (ft_issspace(input[i])) {
            // i++; // Skip spaces
            int start = i;
            while (ft_issspace(input[i])) i++;
            char *tmp = ft_substr(input, start, i - start);
            append_token(&head, create_token(tmp, TOKEN_SPACE));
            free(tmp);
        }
        else if (redir_pipe(input[i]))
            i = handle_redir(input, &head, i);
        else
            i = add_word(input, &head, i);
        if (i < 0)
        {
            // free_token_list(head);
            return NULL; // Error in tokenization
        }
    }
    return head;
}
t_token *tokenize2(char *input)
{
    t_token *head = NULL;
    int i = 0;

    while (input[i] && ft_issspace(input[i]))
        i++; // Skip leading spaces
    while (input[i])
    {
        if (ft_issspace(input[i])) {
            i++; // Skip ?
        }
        else if (redir_pipe(input[i]))
            i = handle_redir(input, &head, i);
        else
            i = add_word(input, &head, i);
        if (i < 0)
        {
            // free_token_list(head);
            return NULL; // Error in tokenization
        }
    }
    return head;
}
t_token *tokenize1(char *input)
{
    t_token *head = NULL;
    int i = 0;

    // while (input[i] && ft_issspace(input[i]))
    //     i++; // Skip leading spaces
    while (input[i])
    {
        if (ft_issspace(input[i])) {
            // i++; // Skip spaces
            printf("space found at %d\n", i);
            int start = i;
            while (ft_issspace(input[i])) i++;
            printf("space found at %d\n", i);
            char *tmp = ft_substr(input, start, i - start);
            append_token(&head, create_token(tmp, TOKEN_SPACE));
            free(tmp);
        }
        else
            i = add_word1(input, &head, i);
        if (i < 0)
        {
            // free_token_list(head);
            return NULL; // Error in tokenization
        }
    }
    return head;
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
    if (!str)
        return NULL;

    size_t len = ft_strlen(str);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;

    size_t i = 0, j = 0;
    while (str[i])
    {
        // Handle empty double quotes
        if (str[i] == '"' && str[i + 1] == '"')
        {
            result[j++] = ' ';  // Replace "" with space
            i += 2;
        }
        // Handle empty single quotes
        else if (str[i] == '\'' && str[i + 1] == '\'')
        {
            result[j++] = ' ';  // Replace '' with space
            i += 2;
        }
        // Handle escaped quotes
        else if (str[i] == '\\' && 
                (str[i + 1] == '\'' || str[i + 1] == '"' || str[i + 1] == '\\'))
        {
            result[j++] = str[i + 1];
            i += 2;
        }
        // Handle regular quoted content
        else if (str[i] == '\'' || str[i] == '"')
        {
            char quote = str[i++];
            while (str[i] && str[i] != quote)
                result[j++] = str[i++];
            if (str[i] == quote)
                i++;
        }
        // Normal character
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
                expand_herdoc(target, env);
                printf("Expanding: %s\n", target->value);
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

t_token *handle_tokenz_expansion(char *qote)
{
    t_token *tokens = NULL;
    int i = 0;

    while (qote[i])
    {
        // Skip whitespace and emit space token
        if (ft_issspace(qote[i]))
        {
            while (qote[i] && ft_issspace(qote[i]))
                i++;
            append_token(&tokens, create_token(ft_strdup(" "), TOKEN_SPACE));
        }
        else
        {
            int start = i;
            int in_single = 0, in_double = 0;

            while (qote[i])
            {
                if (!in_single && qote[i] == '"' && !in_double)
                    in_double = 1;
                else if (in_double && qote[i] == '"')
                    in_double = 0;
                else if (!in_double && qote[i] == '\'' && !in_single)
                    in_single = 1;
                else if (in_single && qote[i] == '\'')
                    in_single = 0;

                if (!in_single && !in_double && ft_issspace(qote[i]))
                    break;

                i++;
            }

            char *word = ft_substr(qote, start, i - start);
            append_token(&tokens, create_token(word, TOKEN_WORD));
            free(word);
        }
    }

    return tokens;
}


// t_token *token_space(t_token *tokens)
// {
//     t_token *head = NULL;
//     t_token *last = NULL;

//     while (tokens)
//     {
//         if (tokens->type == TOKEN_SPACE)
//         {
//             if (!head || last->type != TOKEN_SPACE)
//             {
//                 t_token *new_token = create_token(" ", TOKEN_SPACE);
//                 append_token(&head, new_token);
//                 last = new_token;
//             }
//         }
//         else
//         {
//             append_token(&head, tokens);
//             last = tokens;
//         }
//         tokens = tokens->next;
//     }

//     return head;
// }
void free_tokens(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}
void free_cmd(t_cmd *cmd)
{
    t_cmd *tmp;
    while (cmd)
    {
        tmp = cmd;
        cmd = cmd->next;
        if (tmp->cmd)
        {
            for (int i = 0; tmp->cmd[i]; i++)
                free(tmp->cmd[i]);
            free(tmp->cmd);
        }
        if (tmp->args)
        {
            for (int i = 0; tmp->args[i]; i++)
                free(tmp->args[i]);
            free(tmp->args);
        }
        if (tmp->files)
        {
            for (int i = 0; tmp->files[i]; i++)
                free(tmp->files[i]);
            free(tmp->files);
        }
        if (tmp->redirection)
        {
            for (int i = 0; tmp->redirection[i]; i++)
                free(tmp->redirection[i]);
            free(tmp->redirection);
        }
        free(tmp);
    }
}
t_cmd *parse(t_env *env, t_heredoc *heredoc, t_cmd *cmd_1)
{
    t_token *tokens;
    t_cmd *cmd = NULL;
    char *input;

    input = read_input("minishell$ ");
    tokens = tokenize2(input);
    free(input);
    if (!tokens)
        return NULL;
    mark_file_tokens(tokens);
    if (!error(tokens, heredoc))
    {
        free_tokens(tokens);
        return NULL;
    }
    if(check_ambiguous_redirection(tokens, env) == 0)
    {
        free_tokens(tokens);
        return NULL;
    }
    // tokens = tokenize(input);
    tokens = expand(tokens, env, cmd_1);
    if (!tokens)
    {
        free_tokens(tokens);
        return NULL;
    }
    t_token *tmp = tokens;
    while (tmp)
    {
        // printf("Value: %s\t ", tmp->value);
        // print_type(tmp->type);
        tmp = tmp->next;
    }
   cmd = joining2(tokens);
   if (!cmd)
   {
       free_tokens(tokens);
       free_cmd(cmd);
       return NULL;
   }
   free_tokens(tokens);
    remove_quotes_cmd(cmd);
    return cmd;
}
