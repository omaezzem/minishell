/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:14:49 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/03 17:49:05 by omaezzem         ###   ########.fr       */
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
	// if (ft_strcmp(str, '"') == 0)
	// 	return TOKEN_DQUOTE;
	if (ft_strcmp(str, ">") == 0)
		return TOKEN_REDIRECT_OUT;
	if (ft_strcmp(str, "(") == 0)
		return TOKEN_OPEN_PAREN;
	if (ft_strcmp(str, ")") == 0)
		return TOKEN_CLOSE_PAREN;
	if (str[0] == '-' || str[0] == '~')
		return TOKEN_OPTION;
	if (ft_strcmp(str, "~/") == 0)
		return TOKEN_OPTION;
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
	// else if (type == TOKEN_DQUOTE)
	// 	printf("TOKEN_DQUOTE\n");
	else if (type == TOKEN_OPEN_PAREN)
		printf("TOKEN_OPEN_PAREN\n");
	else if (type == TOKEN_CLOSE_PAREN)
		printf("TOKEN_CLOSE_PAREN\n");
	else if (type == TOKEN_OPTION)
		printf("TOKEN_OPTION\n");
	else if (type == TOKEN_FILE)
		printf("TOKEN_FILE\n");
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
			if (tokens->next->type == TOKEN_WORD)
				tokens->next->type = TOKEN_FILE;
		}
		tokens = tokens->next;
	}
}

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        if (issspace(input[i])) {
            i++;  // Skip white spaces
            continue;
        }

        // Handle quotes (single and double quotes as separate tokens)
        if (input[i] == '"' || input[i] == '\'')
        {
            char quote = input[i];  // Determine whether it's single or double quote
            int start = i;
            i++;  // Skip the opening quote

            // Capture everything inside the quotes until we find the closing quote
            while (input[i] && input[i] != quote) {
                i++;
            }

            // If we reach the end of the string and no closing quote, print error
            if (input[i] == '\0') {
                printf("minishell: syntax error: unmatched quote '%c'\n", quote);
                return NULL;  // Return NULL indicating an error
            }

            // Otherwise, we found the closing quote, so move past it
            i++;  // Skip the closing quote

            // Optionally, you can add the quoted string as a token here
            char *quoted = ft_substr(input, start, i - start);  // Extract the quoted string
            append_token(&tokens, create_token(quoted, get_token_type(quoted)));
            free(quoted);  // Free the extracted quoted string
            continue;
        }

        // Handle double-character operators (e.g., `>>`, `||`, etc.)
        if (ft_strchr("><|& ~/", input[i]) && input[i] == input[i + 1])
        {
            char op[3] = {input[i], input[i + 1], '\0'};
            append_token(&tokens, create_token(op, get_token_type(op)));
            i += 2;
            continue;
        }

        // Handle single-character operators (e.g., `<`, `>`, `|`, `(`, `)`)
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
t_cmd *parse(t_env *env)
{
    t_token *tokens;
    t_cmd *cmd = NULL;
	// t_env *env_list = NULL;
    char *input;

    input = read_input("minishell$ ");
    tokens = NULL;
    tokens = tokenize(input);
    if (!tokens)
    {
        free(input);
        return NULL;
    }
    mark_file_tokens(tokens); // Mark tokens related to file operations (if any)
    free(input);  // We don't need input anymore after tokenization

    t_token *tmp = tokens;
    while (tmp)
    {
        printf("Value: %s\t", tmp->value);
        print_type(tmp->type); // You may want to implement this function to show token types
        tmp = tmp->next;
    }

    // Syntax error handling for unexpected PIPE token
    if (tokens->type == TOKEN_PIPE)
    {
        printf("minishell: syntax error near unexpected token '%s'\n", tokens->value);
        return NULL;
    }

    // Perform error checks
    if (!error(tokens)) return NULL;
    if (!error_pipe(tokens)) return NULL;

    // Join tokens into a command structure
    cmd = joining(tokens);
    if (!cmd) return NULL;

    // Expand environment variables in tokens
    // cmd->env = ft_create_env(env, &env_list);  // Populate cmd->env with the environment list
    // printf("env: %s\n", cmd->env);
    // while (env)
    // {
    //     printf("var: %s\n", env->var);
    //     printf("val: %s\n", env->val);
    //     env = env->next;
    // }

    if (!expand(tokens, env))
    {
        printf("Expansion error\n");
        return NULL;
    }

    // Now, let's set up the environment list in the cmd structure

    // You can now access environment variables using cmd->env in other parts of the program
    // For example, print the first environment variable's value:
    // if (cmd->env)
    // {
    //     printf("First env variable: %s=%s\n", cmd->env->var, cmd->env->val);
    // }

    // Optionally, free the tokens list
    // free_token_list(tokens);

    return cmd;
}
