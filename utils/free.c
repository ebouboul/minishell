/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:46:26 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:55:52 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_memory(t_MemoryManager *manager, void *ptr)
{
	t_MemoryNode	*new_node;

	new_node = (t_MemoryNode *)malloc(sizeof(t_MemoryNode));
	if (!new_node)
	{
		perror("Failed to allocate memory for tracking node");
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->next = manager->head;
	manager->head = new_node;
}

void	remove_memory(t_MemoryManager *manager, void *ptr)
{
	t_MemoryNode	*current;
	t_MemoryNode	*previous;

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

void	*gc_malloc(t_MemoryManager *manager, size_t size)
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

void	gc_free_all(t_MemoryManager *manager)
{
	t_MemoryNode	*current;
	t_MemoryNode	*next_node;

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

void	gc_free(t_MemoryManager *manager, void *ptr)
{
	remove_memory(manager, ptr);
	if (ptr != NULL)
		free(ptr);
}
