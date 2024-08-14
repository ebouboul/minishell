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
    char *result = (char*)malloc(strlen(s) + 2);
    if (result == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    result[0] = c;
    strcpy(result + 1, s);
    return result;
}


char **split_by_quots(char *input)
{
    int i = 0;
    while(input[i] && (input[i] != '"' || input[i] != '\''))
    i++;
    if(input[i] == '"')
        {
            char **result = ft_split(input, '"');
            i = 0;
            while (result[i]) {
                result[i] = ft_strjoin(result[i], result[i + 1]);
                i++;
            }
            i = 0;
            while (result[i]) {
                printf("%s\n", result[i]);
                i++;
            }
            i = 0;
            while (result[i]) {
                printf("%s\n", result[i]);
                i++;
            }
            return result;
        }
    else if(input[i] == '\'')
        {
            char **result = ft_split(input, '\'');
            int i = 0;
            while (result[i]) {
                result[i] = ft_strjoin(result[i], result[i + 1]);
                i++;
            }
            i = 0;
            while (result[i]) {
                printf("%s\n", result[i]);
                i++;
            }
            return result;
        }
    else
        return NULL;

}


// char **split_by_quots(char **input, char c)
// {
//     char **result;
//     int k = 0;
//     while(input[k])
//     {
//         result = ft_split(input[k], c);
//         k++;
//     }
//     int i = 0;
//     while (result[i]) {
//         result[i] = ft_strjoin(result[i], result[i + 1]);
//         i++;
//     }
//     return result;

// }

int truck_quots(char *input, char c)
{
    int i;
    int k;
    i = 0;
    k = 0;
    while(input[i])
    {
        if (input[i] == c)
            k++;
        i++;
    }
    if(k % 2 != 0)
        {
            printf("syntax Error: --%c--\n", c);
            return 1;
        }
    else
        return 0;
}

// int check_RRAH(TokenNode *head)
// {
//     TokenNode *current = head;
//     while (current != NULL) {
//         if (current->info.type == TOKEN_APPEND || current->info.type == TOKEN_REDIRECT_OUT 
//         || current->info.type == TOKEN_REDIRECT_IN || current->info.type == HEREDOC)
//         {
//             if (current->next == NULL || current->next->info.type != TOKEN_FILE)
//             {
//                 printf("syntax Error: --%s--\n", current->info.value);
//                 return 1;
//             }
//         }
//         current = current->next;
//     }
//     return 0;
// }