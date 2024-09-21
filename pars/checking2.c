/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:25:43 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:45:53 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(t_TokenNode *head, char c)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_COMMAND
			|| current->info.type == TOKEN_ARG)
		{
			if (current->info.value[0] == c
				&& current->info.value[strlen(current->info.value) - 1] != c)
				return (print_error("Error: quotes not closed\n"));
		}
		current = current->next;
	}
	return (0);
}

int	check_special_validity(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_COMMAND)
		{
			if (strchr(current->info.value, '<') || strchr(current->info.value,
					'>'))
				return (print_error("Error: specialcharacters not allowed\n"));
		}
		current = current->next;
	}
	return (0);
}

int	skip_spaces(char *input, int index)
{
	while (is_space(input[index]))
		index++;
	return (index);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == ';');
}
