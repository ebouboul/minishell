/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:17:11 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/18 18:53:25 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int count_heredoc(TokenNode *list_head)
{
    int count = 0;
    TokenNode *temp = list_head;
    while (temp)
    {
        if (temp->info.type == TOKEN_HEREDOC)
            count++;
        temp = temp->next;
    }
    if (count > 16)
        return 2;
    return 0;
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
    else if(count_heredoc(list_head) == 2)
    {
        write(2, "minishell: too many heredocuments\n", 34);
        exit(2);
    }
    return 0;
}
TokenInfo *process_input(char *input, int *exit_status, MemoryManager *manager)
{
    char **inp;
    TokenInfo *tokens;

    input = add_spaces(input, manager);
    inp = ft_split(input, manager);
    tokens = tokenizer(inp, manager);
    if (!tokens) 
        return NULL;
    TokenNode *list_head = ArrayIntoNodes(tokens, manager);
    if (checking(list_head) == 2)
    {
        *exit_status = 2;
        return NULL;
    }
    
    return tokens;
}

t_node *prepare_execution(TokenInfo *tokens, t_env *env_list, int exit_status, MemoryManager *manager)
{
    TokenNode *list_head = ArrayIntoNodes(tokens, manager);
    t_node *node = convert_to_node_list(list_head, manager);

    expansion_process(&node, env_list, exit_status, manager);
    
    // print_node_list(node);
    replace_wildcard_in_args(node, manager);
    remove_quotes_and_join(node, manager);
    return node;
}

void increment_shlvl(t_env *env_list, MemoryManager *manager)
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
            new_value = ft_itoa(value, manager);
            gc_free(manager, current->env->value);
            current->env->value = new_value;
            break;
        }
        current = current->next;
    }
}
void whiling(t_node *node, t_env *env_list, MemoryManager *manager)
{
    char *input;
    input = NULL;
    while (1) 
    {
        input = read_user_input();
        if (input == NULL) 
        {
            write(1, "exit\n", 5);
            break;
        }
        if (!input || is_space1(input) == 1)
            continue;
        if(validate_input(input, &node->exit_status))
        {
            TokenInfo *tokens;
            tokens = process_input(input, &node->exit_status, manager);
            free(input);
            if (!tokens)
                continue;
        node = prepare_execution(tokens, env_list, node->exit_status, manager);
        execute_cmds(node, &env_list, &node->exit_status, manager);
        }
    }
}

int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;
    MemoryManager *manager = (MemoryManager *)malloc(sizeof(MemoryManager));
   
    manager->head = NULL;
    
    t_env *env_list = (t_env*)gc_malloc(manager, sizeof(t_env));
    t_node *node = (t_node*)gc_malloc(manager, sizeof(t_node));
    node->exit_status = 0;
    fill_env_list(manager, env, env_list);
    increment_shlvl(env_list, manager);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, handler);
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    // if (sigaction(SIGINT, &sa, NULL) == -1)
    // {
    //     perror("sigaction");
    //     return 1;
    // }
    whiling(node, env_list, manager);
    gc_free_all(manager);
    free(manager);
    rl_clear_history();
    return 0;
}






