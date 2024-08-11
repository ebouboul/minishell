#include "minishell.h"

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
    char *result = (char*)malloc(strlen(s) + 2);
    if (result == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    result[0] = c;
    strcpy(result + 1, s);
    return result;
}


char **split_by_quots(char *input, char c)
{
    char **result = ft_split(input, c);
    int i = 0;
    while (result[i]) {
        result[i] = ft_strjoin(result[i], result[i + 1]);
        i++;
    }
    return result;

}
