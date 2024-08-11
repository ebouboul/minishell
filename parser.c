#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TokenNode *create_node(TokenInfo token)
{
    TokenNode *new_node = (TokenNode*)malloc(sizeof(TokenNode));
    if (new_node == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    new_node->info = token;
    new_node->next = NULL;
    return new_node;
}

TokenNode *ArrayIntoNodes(TokenInfo *tokens)
{
    if (tokens == NULL || tokens[0].type == TOKEN_EOF) {
        return NULL;
    }

    TokenNode *head = create_node(tokens[0]);
    TokenNode *current = head;
    int i = 1;

    while (tokens[i].type != TOKEN_EOF) {
        current->next = create_node(tokens[i]);
        current = current->next;
        i++;
    }

    return head;
}

// Function to print the linked list (for debugging)
void print_linked_list(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) {
        printf("Token: %s, Type: %d\n", current->info.value, current->info.type);
        current = current->next;
    }
}
