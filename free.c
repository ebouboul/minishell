/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 02:37:14 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/29 03:21:06 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent each allocated memory block
typedef struct MemoryNode{
    void *ptr; // Pointer to the allocated memory
    struct MemoryNode *next; // Pointer to the next node in the list
} MemoryNode;

// Head pointer for the linked list of memory blocks
MemoryNode *head = NULL;

// Function to add a memory block to the tracking list
void add_memory(void *ptr) {
    MemoryNode *new_node = (MemoryNode *)malloc(sizeof(MemoryNode));
    if (!new_node) {
        perror("Failed to allocate memory for tracking node");
        exit(EXIT_FAILURE);
    }
    new_node->ptr = ptr;
    new_node->next = head;
    head = new_node;
}

// Function to remove a memory block from the tracking list
void remove_memory(void *ptr)
{
    MemoryNode *current = head;
    MemoryNode *previous = NULL;

    while (current != NULL) {
        if (current->ptr == ptr) {
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current); // Free the tracking node
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Function to allocate memory and add it to the tracking list
void *gc_malloc(size_t size) 
{
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    add_memory(ptr);
    return ptr;
}

// Function to free all allocated memory blocks
void gc_free_all() 
{
    MemoryNode *current = head;
    while (current != NULL) {
        MemoryNode *next_node = current->next;
        if(current->ptr != NULL)
        free(current->ptr); // Free the allocated memory
        if(current != NULL)
        free(current); // Free the tracking node
        current = next_node;
    }
    head = NULL; // Reset the head pointer
}

// Function to free a specific memory block
void gc_free(void *ptr)
{
    if(ptr != NULL)
    free(ptr); // Free the allocated memory
    remove_memory(ptr); // Remove it from the tracking list
}

