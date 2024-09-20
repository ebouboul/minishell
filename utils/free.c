/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:46:26 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/20 02:50:04 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_memory(MemoryManager *manager, void *ptr)
{
	MemoryNode	*new_node;

	new_node = (MemoryNode *)malloc(sizeof(MemoryNode));
	if (!new_node)
	{
		perror("Failed to allocate memory for tracking node");
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->next = manager->head;
	manager->head = new_node;
}

void	remove_memory(MemoryManager *manager, void *ptr)
{
	MemoryNode	*current;
	MemoryNode	*previous;

	current = manager->head;
	previous = NULL;
	while (current != NULL)
	{
		if (current->ptr == ptr)
		{
			if (previous == NULL)
			{
				manager->head = current->next;
			}
			else
			{
				previous->next = current->next;
			}
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	*gc_malloc(MemoryManager *manager, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	add_memory(manager, ptr);
	return (ptr);
}

void	gc_free_all(MemoryManager *manager)
{
	MemoryNode	*current;
	MemoryNode	*next_node;

	current = manager->head;
	while (current != NULL)
	{
		next_node = current->next;
		if (current->ptr != NULL)
			free(current->ptr);
		free(current);
		current = next_node;
	}
	manager->head = NULL;
}

void	gc_free(MemoryManager *manager, void *ptr)
{
	remove_memory(manager, ptr);
	if (ptr != NULL)
		free(ptr);
}
