#include "minishell.h"

int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;
    TokenNode *list_head = NULL;
    TokenInfo *tokens;
    t_env *env_list = (t_env*)malloc(sizeof(t_env));
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
        if(strcmp(input, "env") == 0)
        {
            int i = 0;
            while(env[i])
            {
                printf("%s\n", env[i]);
                i++;
            }
        }
        // if(strncmp(input, "unset", 5) == 0)
        // {
        //     unset_env(env, input + 6);
        // }
        // if(strncmp(input, "export", 6) == 0)
        // {
            // fill_env_list(env, env_list);
        //     // print_env_list(env_list);
        //     // check_key_if_deja(env_list, input + 7);
        //     print_env_list(env_list);
        // }
        truck_quots(input, '"');
        truck_quots(input, '\'');
        add_spaces(input);
        inp = ft_split(input, ' ');
        // char **sp = split_by_quots(input, '"');
        // char **sp2 = split_by_quots(input, '\'');
        // int i = 0;
        // while (sp[i])
        // printf("%s\n", sp[i++]);
        

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
        // expansion_process(list_head, env_list);
        print_linked_list(list_head);
        // check_quotes_spiclal_chars(list_head, '"');
        // check_quotes_spiclal_chars(list_head, '\'');
        check_special_validity(list_head);
        check_syntax_special_Face_to_Face(list_head);
        fill_env_list(env, env_list);
        check_value_env(inp[0], env_list);

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