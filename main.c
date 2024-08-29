#include "minishell.h"

int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;
    TokenNode *list_head = NULL;
    TokenInfo *tokens;
    t_node *node;
    t_env *env_list = (t_env*)malloc(sizeof(t_env));
    char *input;
    char **inp;
    int i = 0;
  
    fill_env_list(env, env_list);
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
        if(truck_quots(input, '"') == 1 || truck_quots(input, '\'') == 1)
        {
            perror("Error: quotes not closed\n");
            continue;
        }
        inp = ft_split(input);


        tokens = tokenizer(inp);
        if (tokens == NULL) 
        {
            return 1;
        }

        TokenNode *list_head = ArrayIntoNodes(tokens);
        if (list_head == NULL) 
        {
            perror("Memory allocation failed\n");
            return 1;
        }
        check_special_chars(list_head);


        check_syntax_double_special_charcters(list_head);
        check_syntax_special_Face_to_Face(list_head);
        node = convert_to_node_list(list_head);
        remove_quotes_and_join(node);
        execute_builtin(node, &env_list);
        // execute_commands(node, &env_list);
        print_node_list(node);
        check_special_validity(list_head);

    }



//  printf("Linked List of Tokens:\n");
 


    free_linked_list(list_head);


     i = 0;
   
    free(inp);
    free(tokens);
    return 0;
}
