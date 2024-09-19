/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:17:08 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 03:32:10 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_tokens(TokenInfo *tokens, int c,
		MemoryManager *manager)
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

int	handle_regular_token(TokenData *data)
{
	int	type;

	if (data->c > 0)
		type = get_token_type_from_previous(&data->tokens[data->c
				- 1]);
	else
		type = TOKEN_COMMAND;
	data->c = create_token(data, type);
	return (data->c);
}

int	tokenize_line(TokenData *data)
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

TokenInfo	*allocate_tokens(MemoryManager *manager)
{
	TokenInfo	*tokens;

	tokens = (TokenInfo *)gc_malloc(manager, 100 * sizeof(TokenInfo));
	if (!tokens)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

TokenInfo	*tokenizer(char **inputs, MemoryManager *manager)
{
	TokenInfo	*tokens;
	int			j;
	TokenData	data;

	j = 0;
	data.c = 0;
	tokens = allocate_tokens(manager);
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
