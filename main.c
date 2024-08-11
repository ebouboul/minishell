#include "minishell.h"

int main() {
    char input[1000];
    printf("Enter a command: ");
    if (fgets(input, sizeof(input), stdin) == NULL) 
    {
        perror("Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = 0;

    char **inp = ft_split(input, ' ');


    TokenInfo *tokens = tokenizer(inp);
    if (tokens == NULL) 
    {
        perror("Memory allocation failed\n");
        return 1;
    }

TokenNode *list_head = ArrayIntoNodes(tokens);

printf("Linked List of Tokens:\n");
 print_linked_list(list_head);


    int i = 0;
    while (inp[i])
    {
        free(inp[i++]);
    }
    free(inp);
    

    free(tokens);
    return 0;
}