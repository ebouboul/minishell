#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == ';');
}

TokenType	get_token_type(char *c)
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

int	skip_spaces(char *input, int index)
{
	while (is_space(input[index]))
		index++;
	return (index);
}

int	get_token_type_from_previous(TokenInfo *previous_token)
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

int	create_token(TokenInfo *tokens, char *input, int *i, int *token_count, int type, MemoryManager *manager)
{
	int	k;

	k = 0;
	tokens[*token_count].value = (char *)gc_malloc(manager, ft_strlen(input) + 1);
	if (tokens[*token_count].value == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	tokens[*token_count].type = type;
	while (input[*i] && k < MAX_TOKEN_LENGTH - 1)
	{
		tokens[*token_count].value[k++] = input[(*i)++];
	}
	tokens[*token_count].value[k] = '\0';
	return (++(*token_count));
}

int	handle_special_char(char *input, TokenInfo *tokens, int *token_count, int *i, MemoryManager *manager)
{
	int	start;
	int	length;

	start = *i;
	while (input[*i] && is_special_char(input[*i]))
	{
		(*i)++;
	}
	length = *i - start;
	if (length <= 0)
		return (*token_count);
	tokens[*token_count].value = (char *)gc_malloc(manager, length + 1);
	if (!tokens[*token_count].value)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	strncpy(tokens[*token_count].value, input + start, length);
	tokens[*token_count].value[length] = '\0';
	tokens[*token_count].type = get_token_type(tokens[*token_count].value);
	return (++(*token_count));
}

TokenInfo	*tokenizer(char **inputs, MemoryManager *manager)
{
	TokenInfo	*tokens;
	int		i, j = 0, token_count = 0;

	tokens = (TokenInfo *)gc_malloc(manager, 100 * sizeof(TokenInfo));
	if (!tokens)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	while (inputs[j])
	{
		i = 0;
		while (inputs[j][i] != '\0' && token_count < MAX_TOKENS)
		{
			i = skip_spaces(inputs[j], i);
			if (inputs[j][i] == '\0')
				break;
			if (is_special_char(inputs[j][i]))
				token_count = handle_special_char(inputs[j], tokens, &token_count, &i, manager);
			else
			{
				if (token_count > 0)
					tokens[token_count].type = get_token_type_from_previous(&tokens[token_count - 1]);
				else
					tokens[token_count].type = TOKEN_COMMAND;
				token_count = create_token(tokens, inputs[j], &i, &token_count, tokens[token_count].type, manager);
			}
		}
		j++;
	}
	tokens[token_count].value = (char *)gc_malloc(manager, 1);
	if (tokens[token_count].value == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	tokens[token_count].type = TOKEN_EOF;
	tokens[token_count].value[0] = '\0';

	return tokens;
}
