/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:19:36 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/22 16:31:09 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	error_pipe(t_token *tokens)
{
	t_token *cur = tokens;

	while (cur)
	{
		if ((cur->type == TOKEN_PIPE && !cur->next) ||
			(cur->type == TOKEN_OR) ||
			(cur->type == TOKEN_PIPE &&
			(cur->next->type == TOKEN_SPACE &&cur->next->next->type == TOKEN_REDIRECT_OUT)) ||
			(cur->type == TOKEN_PIPE && cur->next->type == TOKEN_REDIRECT_OUT))
		{
			printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
			return 0;
		}

		cur = cur->next;
	}
	return 1;
}
char *strip2_quotes(char *str)
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
void remove_quotes2_token(t_token *token)
{
    while (token)
    {
        // Remove quotes from value
        if (token->value)
        {
            char *stripped = strip2_quotes(token->value);
            free(token->value);
            token->value = stripped;
        }

        token = token->next;
    }
}
int handle_heredoc(t_token *delimiter, t_env *env)
{
	// 0) If the user literally typed << "" or << '' with no content,
	//    that’s a syntax error (bash prints “warning: here-document delimited by end-of-file”).
	if (delimiter->value[0] == '\0')
	{
		fprintf(stderr, "minishell: warning: here-document delimited by end-of-file\n");
		return -1;  // or whatever signals “stop processing this heredoc”
	}
	int fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0) { perror("open"); exit(1); }
    char *tmp = ft_strdup(delimiter->value);
	while (1)
    {
        char *line = readline("> ");
        if (!line) {
            close(fd);
            return -1;
        }
        if (ft_strcmp(line, delimiter->value) == 0) {
            free(line);
            break;
        }

        // —— NEW: expand variables in the line ——
        char *to_write = line;     // default: raw
        if (ft_strchr(line, '$')) {   // only if it actually contains a $
            // 1) tokenize the line
            t_token *toks = tokenize(line);
            // 2) run your expand() to get a newly malloc’ed string
            if (!ft_strchr(tmp, '"'))
                to_write = expand(toks, env);
	remove_quotes2_token(delimiter);
            printf("to_write: %s\n", to_write);
            // 3) clean up the token list
            // free_token_list(toks);
            // 4) free the raw line
            free(line);
        }
        // now write either the expanded or the original
        write(fd, to_write, ft_strlen(to_write));
        write(fd, "\n", 1);
        // if we did expand(), we need to free that result too
        if (to_write != line)
            free(to_write);
    }

    return fd;
}
// int handle_heredoc(t_token *delimiter, t_env *env)
// {
// 	// 0) If the user literally typed << "" or << '' with no content,
// 	//    that’s a syntax error (bash prints “warning: here-document delimited by end-of-file”).
// 	if (delimiter->value[0] == '\0')
// 	{
// 		fprintf(stderr, "minishell: warning: here-document delimited by end-of-file\n");
// 		return -1;  // or whatever signals “stop processing this heredoc”
// 	}
// 	remove_quotes2_token(delimiter);
// 	int fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
// 	if (fd < 0) { perror("open"); exit(1); }

// 	while (1)
// 	{
// 		char *line = readline("> ");
// 		if (!line) {
// 			close(fd);
// 			return -1;
// 		}
// 		if (ft_strcmp(line, delimiter->value) == 0) {
// 			free(line);
// 			break;
// 		}

// 		// —— NEW: expand variables in the line ——
// 		char *to_write = line;     // default: raw
// 		if (ft_strchr(line, '$')) {   // only if it actually contains a $
// 			// 1) tokenize the line
// 			t_token *toks = tokenize(line);
// 			// 2) run your expand() to get a newly malloc’ed string
// 			to_write = expand(toks, env);
// 			// 3) clean up the token list
// 			// free_token_list(toks);
// 			// 4) free the raw line
// 			free(line);
// 		}
// 		// now write either the expanded or the original
// 		write(fd, to_write, ft_strlen(to_write));
// 		write(fd, "\n", 1);
// 		// if we did expand(), we need to free that result too
// 		if (to_write != line)
// 			free(to_write);
// 	}

// 	close(fd);
// 	return 0;
// }
int error2(t_token *cur)
{
	if (cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
		return 0;
	}
	if (cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_REDIRECT_IN)
	{
		printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
		return 0;
	}
	return 1;
}
int error(t_token *tokens, t_env *env)
{
    t_token *cur = tokens;

    while (cur)
    {
        // Handle heredoc
          if (cur->type == TOKEN_HEREDOC)
        {
            // Find the delimiter token
            t_token *delim = cur->next;
            while (delim && delim->type == TOKEN_SPACE)
                delim = delim->next;

            // No filename? Syntax error.
            if (!delim || delim->type != TOKEN_FILE)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return 0;
            }

            // **Empty‐string delimiter** → skip immediately
            if (delim->value[0] == '\0')
            {
                // Advance past both << and the empty‐string token
                cur = delim->next;
                continue;
            }

            // Otherwise, actually read a heredoc
            env->fd = handle_heredoc(delim, env);
            if (env->fd < 0)
            {
                // User pressed Ctrl‑D or error → skip this heredoc
                cur = delim->next;
                continue;
            }
            printf("fd: %d\n", env->fd);
            // On successful heredoc, advance and continue parsing
            cur = delim->next;
            continue;
        }

        // Other syntax errors
		if ((cur->type == TOKEN_REDIRECT_OUT && !cur->next) ||
			(cur->type == TOKEN_REDIRECT_IN && !cur->next) ||
			(cur->type == TOKEN_APPEND && !cur->next) ||
			(cur->type == TOKEN_HEREDOC && !cur->next) ||
			(cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_SPACE && !cur->next->next) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_APPEND) ||
			(cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_APPEND && cur->next->type == TOKEN_APPEND) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_HEREDOC) ||
			(cur->type == TOKEN_HEREDOC && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_REDIRECT_OUT) ||
			(cur->type == TOKEN_REDIRECT_IN && cur->next->type == TOKEN_REDIRECT_IN) ||
			(cur->type == TOKEN_REDIRECT_OUT && cur->next->type == TOKEN_PIPE))
        {
            printf("minishell: syntax error near unexpected token '%s'\n", cur->value);
            return 0;
        }


        // Check other redirection errors
        error2(cur);

        cur = cur->next;
    }

    return 1;
}
