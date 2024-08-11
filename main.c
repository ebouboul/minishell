#include "minishell.h"

int main() {
    TokenNode *list_head = NULL;
    TokenInfo *tokens;
    char *input;
    char **inp;
    while(1)
    {
        input = readline("minishell$ ");
        if (input == NULL) {
            break;
        }
        if (ft_strlen(input) == 0) {
            continue;
        }
        add_history(input);
        add_spaces(input);
        inp = ft_split(input, ' ');
        char **sp = split_by_quots(input, '"');
        // char **sp2 = split_by_quots(input, '\'');
        int i = 0;
        while (sp[i])
        printf("%s\n", sp[i++]);
        

        tokens = tokenizer(sp);
        if (tokens == NULL) 
        {
            perror("Memory allocation failed\n");
            return 1;
        }

        TokenNode *list_head = ArrayIntoNodes(tokens);
        check_special_chars(list_head);
        check_syntax(list_head);
        check_quotes(list_head, '"');
        check_quotes(list_head, '\'');
        print_linked_list(list_head);
        check_quotes_spiclal_chars(list_head, '"');
        check_quotes_spiclal_chars(list_head, '\'');
    }



//  printf("Linked List of Tokens:\n");
 


    free_linked_list(list_head);


    int i = 0;
    while (inp[i])
    {
        free(inp[i++]);
    }
    free(inp);
    

    free(tokens);
    return 0;
}