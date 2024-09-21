/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:14:04 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 02:08:08 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_TokenNode	*create_node(t_TokenInfo token, t_MemoryManager *manager)
{
	t_TokenNode	*new_node;

	new_node = (t_TokenNode *)gc_malloc(manager, sizeof(t_TokenNode));
	new_node->info = token;
	new_node->next = NULL;
	return (new_node);
}

t_TokenNode	*arrayintonodes(t_TokenInfo *tokens, t_MemoryManager *manager)
{
	t_TokenNode	*head;
	t_TokenNode	*current;
	int			i;

	if (tokens == NULL || tokens[0].type == TOKEN_EOF)
		return (NULL);
	head = create_node(tokens[0], manager);
	current = head;
	i = 1;
	while (tokens[i].type != TOKEN_EOF)
	{
		current->next = create_node(tokens[i], manager);
		current = current->next;
		i++;
	}
	return (head);
}

void	print_linked_list(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		printf("Token: %s, Type: %d\n", current->info.value,
			current->info.type);
		current = current->next;
	}
}

void	free_linked_list(t_TokenNode *head)
{
	t_TokenNode	*current;
	t_TokenNode	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

int	track_quots(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '"')
		{
			i++;
			while (input[i] != '\0' && input[i] != '"')
				i++;
			if (input[i] == '\0')
				return (print_error("Error: quotes not closed\n"));
		}
		if (input[i] == '\'')
		{
			i++;
			while (input[i] != '\0' && input[i] != '\'')
				i++;
			if (input[i] == '\0')
				return (print_error("Error: quotes not closed\n"));
		}
		i++;
	}
	return (0);
}
