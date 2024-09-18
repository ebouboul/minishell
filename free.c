#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to add a memory block to the tracking list
void add_memory(MemoryManager *manager, void *ptr)
{
    MemoryNode *new_node = (MemoryNode *)malloc(sizeof(MemoryNode));
    if (!new_node)
    {
        perror("Failed to allocate memory for tracking node");
        exit(EXIT_FAILURE);
    }
    new_node->ptr = ptr;
    new_node->next = manager->head;
    manager->head = new_node;
}

// Function to remove a memory block from the tracking list
void remove_memory(MemoryManager *manager, void *ptr)
{
    MemoryNode *current = manager->head;
    MemoryNode *previous = NULL;

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
            free(current); // Free the tracking node
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Function to allocate memory and add it to the tracking list
void *gc_malloc(MemoryManager *manager, size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    add_memory(manager, ptr);
    return ptr;
}

// Function to free all allocated memory blocks
void gc_free_all(MemoryManager *manager)
{
    MemoryNode *current = manager->head;
    while (current != NULL)
    {
        MemoryNode *next_node = current->next;
        if (current->ptr != NULL)
            free(current->ptr); // Free the allocated memory
        free(current);          // Free the tracking node
        current = next_node;
    }
    manager->head = NULL; // Reset the head pointer
      // Free the manager itself
}

// Function to free a specific memory block
// void gc_free(MemoryManager *manager, void *ptr)
// {
//     if (ptr != NULL)
//         free(ptr); // Free the allocated memory
//     remove_memory(manager, ptr); // Remove it from the tracking list
// }
void gc_free(MemoryManager *manager, void *ptr)
{
    remove_memory(manager, ptr); // Remove it from the tracking list first
    if (ptr != NULL)
        free(ptr); // Free the allocated memory
}


// gc_realloc using gc_malloc and gc_free
// void *gc_realloc(MemoryManager *manager, void *ptr, size_t old_size, size_t new_size)
// {
//     void *new_ptr;

//     if (new_size == 0)
//     {
//         gc_free(manager, ptr);
//         return NULL;
//     }

//     new_ptr = gc_malloc(manager, new_size);
//     if (ptr != NULL)
//     {
//         // Copy only the minimum of the old and new sizes
//         memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
//         gc_free(manager, ptr);
//     }
//     return new_ptr;
// }

void *gc_realloc(MemoryManager *manager, void *ptr, size_t new_size)
{
    if (!ptr)
        return gc_malloc(manager, new_size);  // Equivalent to malloc if ptr is NULL

    // Allocate new memory block
    void *new_ptr = gc_malloc(manager, new_size);
    if (!new_ptr)
        return NULL;  // Allocation failed

    ft_memcpy(new_ptr, ptr, new_size);  // Copy the data

    // Free the old memory block
    gc_free(manager, ptr);

    return new_ptr;
}

