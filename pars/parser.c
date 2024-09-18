/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:14:04 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/18 00:14:23 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

TokenNode *create_node(TokenInfo token, MemoryManager *manager)
{
    TokenNode *new_node = (TokenNode *)gc_malloc(manager, sizeof(TokenNode));
    new_node->info = token;
    new_node->next = NULL;
    return new_node;
}

TokenNode *ArrayIntoNodes(TokenInfo *tokens, MemoryManager *manager)
{
    if (tokens == NULL || tokens[0].type == TOKEN_EOF) 
        return NULL;
    TokenNode *head = create_node(tokens[0], manager);
    TokenNode *current = head;
    int i = 1;

    while (tokens[i].type != TOKEN_EOF) 
    {
        current->next = create_node(tokens[i], manager);
        current = current->next;
        i++;
    }
    return head;
}

// Function to print the linked list (for debugging)
void print_linked_list(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) 
    {
        printf("Token: %s, Type: %d\n", current->info.value, current->info.type);
        current = current->next;
    }
}
void free_linked_list(TokenNode *head)
{
    TokenNode *current = head;
    TokenNode *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
int is_special_redc(char c) 
{
    return  c == '<' || c == '>';
}

char *ft_strjoin1(char c, char *s, MemoryManager *manager)
{
    char *result = (char*)gc_malloc(manager, strlen(s) + 2);
    result[0] = c;
    strcpy(result + 1, s);
    return result;
}

int print_error(char *str)
{
    write(2, str, ft_strlen(str));
    return 2;
}

int track_quots(char *input)
{
    int i = 0;
    while(input[i] != '\0')
    {
        if(input[i] == '"')
        {
            i++;
            while(input[i] != '\0' && input[i] != '"')
                i++;
            if(input[i] == '\0')
                return(print_error("Error: quotes not closed\n"));
        }
        if(input[i] == '\'')
        {
            i++;
            while(input[i] != '\0' && input[i] != '\'')
                i++;
            if(input[i] == '\0')
                return(print_error("Error: quotes not closed\n"));
        }
        i++;
    }

    return 0;
}








