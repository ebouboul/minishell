/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:27:35 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 03:46:29 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_closed_quotes(const char *input, t_MemoryManager *manager)
{
	char	*result;
	char	quote;

	int (i), (j);
	i = 0;
	j = 0;
	result = (char *)gc_malloc(manager, ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	while (input[i] != '\0')
	{
		if (is_quote2(input[i]))
		{
			quote = input[i];
			i++;
			while (input[i] != '\0' && input[i] != quote)
				result[j++] = input[i++];
			if (input[i] == quote)
				i++;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*remove_all_quotes2(const char *input, t_MemoryManager *manger)
{
	char	*result;

	int (i), (j);
	i = 0;
	j = 0;
	result = (char *)gc_malloc(manger, ft_strlen(input) + 1);
	if (!result)
	{
		return (NULL);
	}
	while (input[i] != '\0')
	{
		if (input[i] != '"' && input[i] != '\'')
		{
			result[j++] = input[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes_and_join(t_node *head, t_MemoryManager *manager)
{
	t_node		*current;
	t_command	*current_command;
	char		**args;
	int			i;

	current = head;
	while (current != NULL)
	{
		current_command = current->command;
		while (current_command != NULL)
		{
			args = current_command->args;
			i = 0;
			while (args[i] != NULL)
			{
				args[i] = remove_closed_quotes(args[i], manager);
				i++;
			}
			current_command = current_command->next;
		}
		current = current->next;
	}
}

void	remove_closed(char *input)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	if (!input)
		return ;
	while (input &&  input[i] != '\0')
	{
		if (is_quote2(input[i]))
		{
			quote = input[i];
			i++;
			while (input[i] != '\0' && input[i] != quote)
				input[j++] = input[i++];
			if (input[i] == quote)
				i++;
		}
		else
		{
			input[j++] = input[i++];
		}
	}
	input[j] = '\0';
}
