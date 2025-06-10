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

int handle_redir(t_gc *gc, char *input, t_token **head, int i)
{
	int start = i;

	if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
		i += 2;
	else
		i++;

	char *tmp = gc_substr(gc, input, start, i - start);
	t_token *new_token = create_token(gc, tmp, get_token_type(tmp));
	append_token(head, new_token);
	return i;
}


int skip_quotes(const char *s, int i)
{
    char quote = s[i++];
    while (s[i] && s[i] != quote)
	{
        if (s[i] == '\\' && quote == '\"' && s[i + 1])
            i += 2;
        else
            i++;
    }
    return (s[i] == quote) ? i + 1 : i; // skip closing quote if found
}

int add_word(t_gc *gc, char *input, t_token **head, int i)
{
	int start = i;

	while (input[i] && !ft_issspace(input[i]) && !redir_pipe(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i])
			{
                ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
                return -1;
            }
            i++; // skip closing quote
        }
        else
            i++;
    }
	char *tmp = gc_substr(gc, input, start, i - start);
	if (!tmp)
		return -1;
	append_token(head, create_token(gc, tmp, TOKEN_WORD));
	return i;
}

int add_word1(t_gc *gc, char *input, t_token **head, int i)
{
    int start = i;
    while (input[i] && !ft_issspace(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i]) return -1;
            i++;
        }
        else
            i++;
    }
    char *tmp = gc_substr(gc, input, start, i - start);
    t_token *new_token = create_token(gc, tmp, TOKEN_WORD);
    append_token(head, new_token);
    // free(tmp);
    return i;
}

t_token *tokenize(t_gc *gc, char *input)
{
    t_token *head = NULL;
    int i = 0;
    while (input[i] && ft_issspace(input[i]))
        i++;
    while (input[i])
    {
        if (ft_issspace(input[i]))
        {
            int start = i;
            while (ft_issspace(input[i])) i++;
            char *tmp = gc_substr(gc, input, start, i - start);
            append_token(&head, create_token(gc, tmp, TOKEN_SPACE));
            // free(tmp);
        }
        else if (redir_pipe(input[i]))
            i = handle_redir(gc, input, &head, i);
        else
            i = add_word(gc, input, &head, i);
        if (i < 0)
            return NULL;
    }
    return head;
}

t_token *tokenize2(t_gc *gc, char *input)
{
    t_token *head = NULL;
    int i = 0;
    while (input[i] && ft_issspace(input[i]))
        i++;
    while (input[i])
    {
        if (ft_issspace(input[i]))
            i++;
        else if (redir_pipe(input[i]))
            i = handle_redir(gc, input, &head, i);
        else
            i = add_word(gc, input, &head, i);
        if (i < 0)
            return NULL;
    }
    return head;
}
static int skip_spaces(t_gc *gc, char *input, int i, t_token **head)
{
	int start = i;

	while (ft_issspace(input[i]))
		i++;
	char *tmp = gc_substr(gc, input, start, i - start);
	if (!tmp)
		return -1;
	append_token(head, create_token(gc, tmp, TOKEN_SPACE));
	return i;
}

t_token	*tokenize1(t_gc *gc, char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_issspace(input[i]))
			i = skip_spaces(gc, input, i, &head);
		else
			i = add_word1(gc, input, &head, i);
		if (i < 0)
			return (NULL);
	}
	return (head);
}


#include <string.h>
static int	is_empty_quotes(char *str, int i)
{
	return ((str[i] == '"' && str[i + 1] == '"')
		|| (str[i] == '\'' && str[i + 1] == '\''));
}

static int	is_escaped_quote(char *str, int i)
{
	return (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '"'
		|| str[i + 1] == '\\'));
}

static int	handle_quoted_content(char *str, int i, char *res, int *j)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		res[(*j)++] = str[i++];
	if (str[i] == quote)
		i++;
	return (i);
}

char	*strip_quotes(t_gc *gc, char *str)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	res = gc_malloc(gc, ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_empty_quotes(str, i))
			res[j++] = ' ', i += 2;
		else if (is_escaped_quote(str, i))
			res[j++] = str[i + 1], i += 2;
		else if (str[i] == '\'' || str[i] == '"')
			i = handle_quoted_content(str, i, res, (int *)&j);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

static void	remove_quotes_array(t_gc *gc, char **arr)
{
	int		i;
	char	*stripped;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		stripped = strip_quotes(gc, arr[i]);
		// free(arr[i]);
		arr[i] = stripped;
		i++;
	}
}

void	remove_quotes_cmd(t_gc *gc, t_cmd *cmd)
{
	while (cmd)
	{
		remove_quotes_array(gc, cmd->cmd);
		remove_quotes_array(gc, cmd->args);
		remove_quotes_array(gc, cmd->files);
		remove_quotes_array(gc, cmd->redirection);
		cmd = cmd->next;
	}
}


static int	is_ambiguous(t_token *target, t_env *env)
{
	char	*expanded;

	expanded = NULL;
	if (target->value[0] == '$')
	{
		expand_herdoc(target, env);
		if (!expanded || ft_strcmp(expanded, "") == 0)
		{
			printf("minishell: %s: ambiguous redirect\n", target->value);
			// free(expanded);
			return (1);
		}
		// free(expanded);
	}
	return (0);
}

int	check_ambiguous_redirection(t_token *tokens, t_env *env)
{
	t_token	*cur;
	t_token	*target;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_REDIRECT_IN
			|| cur->type == TOKEN_REDIRECT_OUT
			|| cur->type == TOKEN_APPEND
			|| cur->type == TOKEN_HEREDOC)
		{
			target = cur->next;
			while (target && target->type == TOKEN_SPACE)
				target = target->next;
			if (target && target->type == TOKEN_FILE)
				if (is_ambiguous(target, env))
					return (0);
		}
		cur = cur->next;
	}
	return (1);
}

// void free_tokens(t_token *tokens)
// {
//     t_token *tmp;
//     while (tokens)
//     {
//         tmp = tokens;
//         tokens = tokens->next;
//         free(tmp->value);
//         free(tmp);
//     }
// }
// static void	free_strs(char **strs)
// {
// 	int	i;

// 	i = 0;
// 	if (!strs)
// 		return ;
// 	while (strs[i])
// 	{
// 		free(strs[i]);
// 		i++;
// 	}
// 	free(strs);
// }

// void	free_cmd(t_cmd *cmd)
// {
// 	t_cmd	*tmp;

// 	while (cmd)
// 	{
// 		tmp = cmd;
// 		cmd = cmd->next;
// 		free_strs(tmp->cmd);
// 		free_strs(tmp->args);
// 		free_strs(tmp->files);
// 		free_strs(tmp->redirection);
// 		// free(tmp);
// 	}
// }

t_token	*prepare_tokens(char *input, t_env *env,
			t_heredoc *heredoc, t_cmd *cmd_1, t_gc *gc)
{
	t_token	*tokens;

	if (!input)
		return (NULL);
	tokens = tokenize2(gc, input);
	if (!tokens)
		return (NULL);
	mark_file_tokens(tokens);
	if (!error(gc, tokens, heredoc, env))
		return ( NULL);
	if (!check_ambiguous_redirection(tokens, env))
		return ( NULL);
	tokens = expand(gc, tokens, env, cmd_1);
	if (!tokens)
		return (NULL);
	return (tokens);
}



t_cmd *parse(t_gc *gc, t_env *env, t_heredoc *heredoc, t_cmd **cmd_1)
{
    t_token *tokens;
    char *input;
    char *copy;

    input = read_input("minishell$ ");
    if (!input)
        return (NULL);
    copy = gc_malloc(gc, ft_strlen(input) + 1);
    if (!copy)
        return (NULL);
    strcpy(copy, input);
    free(input);
    
    // Pass the persistent cmd to prepare_tokens
    tokens = prepare_tokens(copy, env, heredoc, *cmd_1, gc);
    if (!tokens)
        return (gc_free_all(gc), NULL);
    
    // Create a new temporary cmd structure for this parse cycle
    t_cmd *new_cmd = joining2(gc, tokens);
    if (!new_cmd)
        return (gc_free_all(gc), NULL);
    
    // Copy the exit status from the persistent cmd to the new cmd
    new_cmd->ex_status = (*cmd_1)->ex_status;
    
    remove_quotes_cmd(gc, new_cmd);
    return (new_cmd);
}