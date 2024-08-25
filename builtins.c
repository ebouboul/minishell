/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/25 21:42:18 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// int ft_echo(TokenNode *head);
// int ft_cd(TokenNode *head, t_env *env_list);
// int ft_pwd();
// // int ft_export(TokenNode *head, t_env *env_list, char **env);
// // int ft_unset(TokenNode *head, t_env *env_list);
// int ft_env(t_env *env_list);
// // void unset_env(t_env *env_list, char *input);
// // void add_env(char **env, char *input);
// char *get_env_value(t_env *env_list, char *key);


int is_builtin(char *command)
{
    if (strcmp(command, "echo") == 0)
        return 1;
    else if (strcmp(command, "cd") == 0)
        return 1;
    else if (strcmp(command, "pwd") == 0)
        return 1;
    else if (strcmp(command, "export") == 0)
        return 1;
    else if (strcmp(command, "unset") == 0)
        return 1;
    else if (strcmp(command, "env") == 0)
        return 1;
    else if (strcmp(command, "exit") == 0)
        return 1;
    return 0;
}

int process_echo_option(char *option, int *option_value)
{
    int i = 0;
    while (option[i] != '\0')
    {
        if (option[i] != 'n')
            return 0;
        i++;
    }
    *option_value = 1;
    return 1;
}

int ft_echo(t_command *command)
{
    int option = 0;
    t_command *current = command;
    while (current != NULL)
    {
        if (current->args[1] != NULL && strcmp(current->args[1], "-n") == 0)
        {
            if (process_echo_option(current->args[1], &option))
                break;
        }
        else
        {
            printf("%s ", current->args[1]);
        }
        current = current->next;
    }
    if (!option)
        printf("\n");
    return 0;
}

char *get_env_value(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (strcmp(current->env->key, key) == 0)
            return current->env->value;
        current = current->next;
    }
    return NULL;
}
int ft_cd(t_command *command, t_env *env_list)
{
    char *path;
    if (command->args[1] == NULL)
    {
        path = get_env_value(env_list, "HOME");
        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    }
    else
    {
        path = command->args[1];
    }
    if (chdir(path) == -1)
    {
        printf("cd: %s: %s\n", path, strerror(1));
        return 1;
    }
    return 0;
}
int ft_pwd()
{
    char *path;
    path = getcwd(NULL, 0);
    if (path == NULL)
    {
        printf("pwd: %s\n", strerror(1));
        return 1;
    }
    printf("%s\n", path);
    free(path);
    return 0;
}


int ft_env(t_env *env_list)
{
    print_env_list(env_list);
    return 0;
}




int ft_exit(t_command *command)
{
    if (command->args[1] != NULL)
    {
        printf("exit\n");
        exit(atoi(command->args[1]));
        return 1;
    }
    return 0;
}
int check_key_from_env(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (strcmp(current->env->key, key) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}
void add_env_node(t_env **current, char *key, char *value)
{

    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    new_node->env = (env *)malloc(sizeof(env));
    new_node->env->key = ft_strdup((const char *)key);
    new_node->env->value = ft_strdup((const char *)value);
    new_node->next = NULL;
    (*current)->next = new_node;
    *current = new_node;
    // printf("%s\n", new_node->env->key);
}




// void add_env_node(t_env **current, char *key, char *value) {
//     t_env *new_node = (t_env *)malloc(sizeof(t_env));
    
//     new_node->env = (env *)malloc(sizeof(env));
//     new_node->env->key = ft_strdup(key);
//     new_node->env->value = ft_strdup(value);
    
//     new_node->next = NULL;
    
//     (*current)->next = new_node;
//     *current = new_node;
// }



int ft_export(t_command *command, t_env **env_list)
{
    t_command *current = command;
    t_env *current_env = *env_list;
    while (current != NULL)
    {
        if (current->args[1] == NULL)
        {
            while (current_env != NULL)
            {
                printf("declare -x %s=\"%s\"\n", current_env->env->key, current_env->env->value);
                current_env = current_env->next;
            }
            return 0;
        }
        else
        {
            get_env_value(*env_list, current->args[1]);
        }
        current = current->next;
    }
    return 0;
}



int ft_unset(t_command *command, t_env **env_list)
{
    t_command *current = command;
    t_env *current_env = *env_list;
    t_env *prev = NULL;
    while (current != NULL)
    {
        if (current->args[1] == NULL)
        {
            printf("unset: not enough arguments\n");
            return 1;
        }
        if (check_key_from_env(*env_list, current->args[1]))
        {
            while (current_env != NULL)
            {
                if (strcmp(current_env->env->key, current->args[1]) == 0)
                {
                    if (prev == NULL)
                    {
                        *env_list = current_env->next;
                        free(current_env->env->key);
                        free(current_env->env->value);
                        free(current_env->env);
                        free(current_env);
                        current_env = *env_list;
                        break;
                    }
                    prev->next = current_env->next;
                    free(current_env->env->key);
                    free(current_env->env->value);
                    free(current_env->env);
                    free(current_env);
                    current_env = prev->next;
                    break;
                }
                prev = current_env;
                current_env = current_env->next;
            }
        }
        current = current->next;
    }
    return 0;
}


int execute_builtin(t_node *head, t_env **env_list)
{
    t_node *current = head;
    int status = 0;
    if (strcmp(current->command->args[0], "echo") == 0)
        status = ft_echo(current->command);
    else if (strcmp(current->command->args[0], "cd") == 0)
        status = ft_cd(current->command, *env_list);
    else if (strcmp(current->command->args[0], "pwd") == 0)
        status = ft_pwd();
    else if (strcmp(current->command->args[0], "export") == 0)
        status = ft_export(current->command, env_list);
    else if (strcmp(current->command->args[0], "unset") == 0)
        status = ft_unset(current->command, env_list);
    else if (strcmp(current->command->args[0], "env") == 0)
        status = ft_env(*env_list);
    else if (strcmp(current->command->args[0], "exit") == 0)
        status = ft_exit(current->command);
    return status;
}
