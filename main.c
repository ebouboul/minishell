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
    // while(env[i])
    // {
    //     printf("%s\n", env[i]);
    //     i++;
    // }
    fill_env_list(env, env_list);
     print_env_list(env_list);
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
        // if(strcmp(input, "env") == 0)
        // {
        //     int i = 0;
        //     while(env[i])
        //     {
        //         printf("%s\n", env[i]);
        //         i++;
        //     }
        // }
        // if(strncmp(input, "unset", 5) == 0)
        // {
        //     unset_env(env, input + 6);
        // }
        // if(strncmp(input, "exlort", 6) == 0)
        // {
        //     fill_env_list(env, env_list);
        //     print_env_list(env_list);
        //     // check_key_if_deja(env_list, input + 7);
        //     // print_env_list(env_list);
        // }


        add_spaces(input);
        inp = ft_split(input);


        tokens = tokenizer(inp);
        if (tokens == NULL) 
        {
            perror("Memory allocation failed\n");
            return 1;
        }

        TokenNode *list_head = ArrayIntoNodes(tokens);
        if (list_head == NULL) 
        {
            perror("Memory allocation failed\n");
            return 1;
        }
        // check_RRAH(list_head);
        check_special_chars(list_head);
        // check_syntax(list_head);
        // check_quotes(list_head, '"');
        // check_quotes(list_head, '\'');
        // print_env_list(env_list);
        // remove_quotes_and_join(list_head);
        node = convert_to_node_list(list_head);
        // printf("exit status: %d\n", node->exit_status);
        // print_node_list(node);
        expansion_process(&node, env_list);
        // print_linked_list(list_head);
        // printf("exit status: %d\n", node->exit_status);
        remove_quotes_and_join(node);
        execute_builtin(node, &env_list);
        print_node_list(node);
        // print_linked_list(list_head);
        // // check_quotes_spiclal_chars(list_head, '"');
        // // check_quotes_spiclal_chars(list_head, '\'');
        check_special_validity(list_head);
        check_syntax_special_Face_to_Face(list_head);

        // check_value_env(inp[0], env_list);

    }



//  printf("Linked List of Tokens:\n");
 


    free_linked_list(list_head);


     i = 0;
   
    free(inp);
    free(tokens);
    return 0;
}