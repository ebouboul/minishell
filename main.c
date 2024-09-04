#include "minishell.h"

char *read_user_input(void) 
{
    char *input = readline("minishell$ ");
    if (input)
        add_history(input);
    return input;
}

int validate_input(char *input, t_node **head)
{
    if (ft_strlen(input) == 0)
        return 0;  // Empty input, skip processing

    (*head)->exit_status = track_quots(input);
    if((*head)->exit_status == 2)
        return 0;  // Invalid input 
    return 1;  // Input is valid
}
int checking(TokenNode *list_head)
{
    if(check_special_chars(list_head) == 2)
        return 2;
    else if(check_syntax_double_special_charcters(list_head) == 2)
        return 2;
    else if(check_syntax_special_Face_to_Face(list_head) == 2)
        return 2;
    else if(check_special_validity(list_head) == 2)
        return 2;
    return 0;
}
TokenInfo *process_input(char *input, t_node **head) 
{
    char **inp;
    TokenInfo *tokens;

    input = add_spaces(input);
    inp = ft_split(input);
    tokens = tokenizer(inp);
    if (!tokens) 
        return NULL;
    TokenNode *list_head = ArrayIntoNodes(tokens);
    (*head)->exit_status = checking(list_head);
    if((*head)->exit_status == 2)
        return NULL;
    
    return tokens;
}

t_node *prepare_execution(TokenInfo *tokens, t_env *env_list) 
{
    TokenNode *list_head = ArrayIntoNodes(tokens);
    t_node *node = convert_to_node_list(list_head);

    expansion_process(&node, env_list);
    // print_node_list(node);
    
    remove_quotes_and_join(node);
    return node;
}

void increment_shlvl(t_env *env_list)
{
    t_env *current = env_list;
    while (current != NULL) 
    {
        if (ft_strcmp(current->env->key, "SHLVL") == 0) 
        {
            int value = ft_atoi(current->env->value);
            value++;
            char *new_value = ft_itoa(value);
            gc_free(current->env->value);
            current->env->value = new_value;
            break;
        }
        current = current->next;
    }
}
int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;
    
    t_env *env_list = (t_env*)gc_malloc(sizeof(t_env));
    t_node *node = (t_node*)gc_malloc(sizeof(t_node));
    char *input = NULL;
    fill_env_list(env, env_list);
    increment_shlvl(env_list);

    while (1) 
    {
        input = read_user_input();
        if (!input)
            break;

        if(validate_input(input, &node))
        {
            TokenInfo *tokens = process_input(input, &node);
            free(input);
            if (!tokens)
                continue;
            node = prepare_execution(tokens, env_list);
            print_node_list(node);
            execute_commands(node, &env_list);
        }
    }
    gc_free_all();
    return 0;
}
