#include "minishell.h"

int is_space1(char *input)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] > 32)
            return 0;
        i++;
    }
    return 1;
}
char *read_user_input(void) 
{
    char *input = readline("minishell$ ");
    if (input)
        add_history(input);
    return input;
}

int validate_input(char *input, int *exit_status) 
{
    if (ft_strlen(input) == 0)
        return 0;

    if (track_quots(input) == 2)
    {
        (*exit_status) = 2;
        return 0;
    }
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
TokenInfo *process_input(char *input, int *exit_status) 
{
    char **inp;
    TokenInfo *tokens;

    input = add_spaces(input);
    inp = ft_split(input);
    tokens = tokenizer(inp);
    if (!tokens) 
        return NULL;
    TokenNode *list_head = ArrayIntoNodes(tokens);
    if (checking(list_head) == 2)
    {
        *exit_status = 2;
        return NULL;
    }
    
    return tokens;
}

t_node *prepare_execution(TokenInfo *tokens, t_env *env_list, int exit_status) 
{
    TokenNode *list_head = ArrayIntoNodes(tokens);
    t_node *node = convert_to_node_list(list_head);
    

    expansion_process(&node, env_list, exit_status);
    
    print_node_list(node);
    replace_wildcard_in_args(node);
    remove_quotes_and_join(node);
    return node;
}

void increment_shlvl(t_env *env_list)
{
    t_env *current;
    int value;
    char *new_value;
    current = env_list;
    while (current != NULL) 
    {
        if (ft_strcmp(current->env->key, "SHLVL") == 0) 
        {
            value = ft_atoi(current->env->value);
            value++;
            new_value = ft_itoa(value);
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
    int exit_status = 0;
    char *input = NULL;
    fill_env_list(env, env_list);
    increment_shlvl(env_list);
    signal(SIGQUIT, SIG_IGN);
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    while (1) 
    {
        // printf("exit status: %d\n", node->exit_status);
        input = read_user_input();
        if (input == NULL) 
        {
            write(1, "exit\n", 5);
            break;
        }

        if (!input || is_space1(input) == 1)
            continue;
        if(validate_input(input, &exit_status))
        {
            TokenInfo *tokens = process_input(input, &exit_status);
            free(input);
            if (!tokens)
                continue;


            node = prepare_execution(tokens, env_list, exit_status);
            execute_cmds(node, &env_list, &exit_status);
        }
    }
    gc_free_all();
    return 0;
}



