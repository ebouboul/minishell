/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:25:43 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 20:35:15 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(TokenNode *head, char c)
{
	TokenNode	*current;

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

int	check_special_validity(TokenNode *head)
{
	TokenNode	*current;

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
