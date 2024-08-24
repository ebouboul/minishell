/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/24 16:06:56 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int ft_echo(TokenNode *head);
int ft_cd(TokenNode *head, t_env *env_list);
int ft_pwd();
// int ft_export(TokenNode *head, t_env *env_list, char **env);
// int ft_unset(TokenNode *head, t_env *env_list);
int ft_env(t_env *env_list);
// void unset_env(t_env *env_list, char *input);
// void add_env(char **env, char *input);
char *get_env_value(t_env *env_list, char *key);


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

int process_echo_option(TokenNode *current, int *option)
{
    int i = 2;
    while (current->info.value[i])
    {
        if (current->info.value[i] != 'n')
        {
            printf("%s ", current->info.value);
            return 1;
        }
        i++;
    }
    *option = 1;
    return 0;
}

int ft_echo(TokenNode *head)
{
    TokenNode *current = head;
    int option = 0;

    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
        {
            if (strncmp(current->info.value, "-n", 2) == 0)
            {
                if (process_echo_option(current, &option))
                {
                    current = current->next;
                    continue;
                }
            }
            else if (current->next != NULL)
                printf("%s ", current->info.value);
            else
                printf("%s", current->info.value);
        }
        current = current->next;
    }
    if (option == 0)
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
int ft_cd(TokenNode *head, t_env *env_list)
{
    TokenNode *current = head;
    char *path;
    if (current == NULL)
    {
        path = get_env_value(env_list, "HOME");
        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    }
    else if (current->next != NULL)
    {
        printf("cd: too many arguments\n");
        return 1;
    }
    else
        path = current->info.value;
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




int ft_exit(TokenNode *head)
{
    TokenNode *current = head;
    if (current == NULL)
        exit(0);
    if (current->next != NULL)
    {
        printf("exit: too many arguments\n");
        return 1;
    }
    exit(atoi(current->info.value));
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



int ft_export(TokenNode *head, t_env **env_list)
{
    TokenNode *current = head;
    char *key;
    char *value;
    t_env *current_env = *env_list;
    // t_env *prev = NULL;

    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
        {
            key = current->info.value;
            if (current->next != NULL && current->next->info.type == TOKEN_ARG)
            {
                value = current->next->info.value;
                current = current->next;
            }
            else
            {
                value = "";
            }
            if (check_key_from_env(*env_list, key))
            {
                while (current_env != NULL)
                {
                    if (strcmp(current_env->env->key, key) == 0)
                    {
                        free(current_env->env->value);
                        current_env->env->value = ft_strdup(value);
                        break;
                    }
                    current_env = current_env->next;
                }
            }
            else
            {
                add_env_node(&current_env, key, value);
                print_env_list(*env_list);
            }
        }
        current = current->next;
    }
    return 0;
}



int ft_unset(TokenNode *head, t_env **env_list)
{
    TokenNode *current = head;
    char *key;

    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
        {
            key = current->info.value;
            t_env *current_env = *env_list;
            t_env *prev = NULL;

            while (current_env != NULL)
            {
                if (strcmp(current_env->env->key, key) == 0)
                {
                    if (prev == NULL)
                    {
                        *env_list = current_env->next;
                    }
                    else
                    {
                        prev->next = current_env->next;
                    }

                    free(current_env->env->key);
                    free(current_env->env->value);
                    free(current_env->env);
                    free(current_env);
                    break;
                }
                prev = current_env;
                current_env = current_env->next;
            }
        }
        current = current->next;
    }
    print_env_list(*env_list);

    return 0;
}


int execute_builtin(TokenNode *head, t_env **env_list) 
{
    TokenNode *current = head;
    if (strcmp(current->info.value, "echo") == 0)
        return ft_echo(current->next);
    else if (strcmp(current->info.value, "cd") == 0)
        return ft_cd(current->next, *env_list);
    else if (strcmp(current->info.value, "pwd") == 0)
        return ft_pwd();
    else if (strcmp(current->info.value, "env") == 0)
        return ft_env(*env_list);
    else if (strcmp(current->info.value, "exit") == 0)
        return ft_exit(current->next);
    else if (strcmp(current->info.value, "export") == 0)
        return ft_export(current->next, env_list);
    else if (strcmp(current->info.value, "unset") == 0)
        return ft_unset(current->next, env_list);
    return 0;
}
