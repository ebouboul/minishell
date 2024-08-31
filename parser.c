#include "minishell.h"

TokenNode *create_node(TokenInfo token)
{
    TokenNode *new_node = (TokenNode*)gc_malloc(sizeof(TokenNode));
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
    if (tokens == NULL || tokens[0].type == TOKEN_EOF) 
        return NULL;
    TokenNode *head = create_node(tokens[0]);
    TokenNode *current = head;
    int i = 1;

    while (tokens[i].type != TOKEN_EOF) 
    {
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

char *ft_strjoin1(char c, char *s)
{
    char *result = (char*)gc_malloc(strlen(s) + 2);
    if (result == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    result[0] = c;
    strcpy(result + 1, s);
    return result;
}





int print_error(char *str)
{
    write(2, str, ft_strlen(str));
    return 2;
}

int truck_quots(char *input)
{
    int i = 0;
    int count = 0;
    int clont2 = 0;
    while (input[i] != '\0') 
    {
        if (input[i] == '"')
            count++;
        if (input[i] == '\'')
            clont2++;
        i++;
    }
    if (count % 2 != 0 || clont2 % 2 != 0)
        return(print_error("Error: quotes not closed\n"));
    return 0;
}





