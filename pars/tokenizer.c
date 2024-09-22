/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:31:01 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/22 21:12:03 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_TokenType	get_token_type(char *c)
{
	if (ft_strcmp(c, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(c, ">>") == 0)
		return (TOKEN_APPEND);
	else if (ft_strcmp(c, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	else if (ft_strcmp(c, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	else if (ft_strcmp(c, "<<") == 0)
		return (TOKEN_HEREDOC);
	else
		return (TOKEN_COMMAND);
}

int	get_token_type_from_pr(t_TokenInfo *previous_token)
{
	if (previous_token->type == TOKEN_REDIRECT_IN
		|| previous_token->type == TOKEN_REDIRECT_OUT
		|| previous_token->type == TOKEN_APPEND
		|| previous_token->type == TOKEN_HEREDOC)
		return (TOKEN_FILE);
	if (previous_token->type == TOKEN_FILE
		|| previous_token->type == TOKEN_COMMAND
		|| previous_token->type == TOKEN_ARG)
		return (TOKEN_ARG);
	return (TOKEN_COMMAND);
}

int	create_token(t_TokenData *data, int type)
{
	int	k;

	k = 0;
	data->tokens[data->c].value = (char *)gc_malloc(data->manager,
			ft_strlen(data->input) + 1);
	data->tokens[data->c].type = type;
	while (data->input[data->i] && k < MAX_TOKEN_LENGTH - 1)
	{
		data->tokens[data->c].value[k++] = data->input[data->i++];
	}
	data->tokens[data->c].value[k] = '\0';
	return (++data->c);
}

int	handle_special_char(t_TokenData *data)
{
	int	start;
	int	length;

	start = data->i;
	while (data->input[data->i] && is_special_char(data->input[data->i]))
	{
		data->i++;
	}
	length = data->i - start;
	if (length <= 0)
		return (data->c);
	data->tokens[data->c].value = (char *)gc_malloc(data->manager,
			length + 1);
	strncpy(data->tokens[data->c].value, data->input + start, length);
	data->tokens[data->c].value[length] = '\0';
	data->tokens[data->c].type = get_token_type(data->tokens[data->c].value);
	return (++data->c);
}

int	count_pipe(char **inputs)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 2;
	while (inputs[i])
	{
		j = 0;
		while (inputs[i][j])
		{
			if (inputs[i][j] == '|')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}
