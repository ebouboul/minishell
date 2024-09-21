/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:17:08 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 07:08:15 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_tokens(t_TokenInfo *tokens, int c,
		t_MemoryManager *manager)
{
	tokens[c].value = (char *)gc_malloc(manager, 1);
	if (tokens[c].value == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	tokens[c].type = TOKEN_EOF;
	tokens[c].value[0] = '\0';
}

int	handle_regular_token(t_TokenData *data)
{
	int	type;

	if (data->c > 0)
		type = get_token_type_from_pr(&data->tokens[data->c
				- 1]);
	else
		type = TOKEN_COMMAND;
	data->c = create_token(data, type);
	return (data->c);
}

int	tokenize_line(t_TokenData *data)
{
	while (data->input[data->i] != '\0' && data->c < MAX_TOKENS)
	{
		data->i = skip_spaces(data->input, data->i);
		if (data->input[data->i] == '\0')
			break ;
		if (is_special_char(data->input[data->i]))
			data->c = handle_special_char(data);
		else
			data->c = handle_regular_token(data);
	}
	return (data->c);
}

t_TokenInfo	*allocate_tokens(t_MemoryManager *manager, int size)
{
	t_TokenInfo	*tokens;

	tokens = (t_TokenInfo *)gc_malloc(manager, (size + 10000) * sizeof(t_TokenInfo));
	if (!tokens)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (tokens);
}
int count_pipe(char **inputs)
{
	int i;
	int j;
	int count;
	
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

t_TokenInfo	*tokenizer(char **inputs, t_MemoryManager *manager)
{
	t_TokenInfo	*tokens;
	int			j;
	t_TokenData	data;

	j = 0;
	data.c = 0;
	tokens = allocate_tokens(manager, count_pipe(inputs));
	data.tokens = tokens;
	data.manager = manager;
	while (inputs[j])
	{
		data.input = inputs[j];
		data.i = 0;
		data.c = tokenize_line(&data);
		j++;
	}
	finalize_tokens(data.tokens, data.c, data.manager);
	return (data.tokens);
}
