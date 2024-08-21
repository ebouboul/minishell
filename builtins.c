/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/21 20:30:54 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int ft_echo(TokenNode *head);
int ft_cd(TokenNode *head, t_env *env_list);
int ft_pwd();
int ft_export(TokenNode *head, t_env *env_list);
int ft_unset(TokenNode *head, t_env *env_list);
int ft_env(t_env *env_list);
void unset_env(t_env *env_list, char *input);
void add_env(t_env *env_list, char *input);
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

int ft_echo(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
        {
            printf("%s", current->info.value);
            if (current->next != NULL)
                printf(" ");
        }
        current = current->next;
    }
    printf("\n");
    return 0;
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
int ft_export(TokenNode *head, t_env *env_list)
{
    TokenNode *current = head;
    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
        {
            if (ft_strchr(current->info.value, '=') == NULL)
            {
                printf("export: '%s': not a valid identifier\n", current->info.value);
                return 1;
            }
            add_env(env_list, current->info.value);
        }
        else if (current->info.type == TOKEN_COMMAND)
        {
            char *value = get_env_value(env_list, current->info.value);
            if (value != NULL)
                printf("declare -x %s=\"%s\"\n", current->info.value, value);
        }
        current = current->next;
    }
    return 0;
}
int ft_unset(TokenNode *head, t_env *env_list)
{
    TokenNode *current = head;
    while (current != NULL)
    {
        if (current->info.type == TOKEN_ARG)
            unset_env(env_list, current->info.value);
        current = current->next;
    }
    return 0;
}
int ft_env(t_env *env_list)
{
    print_env_list(env_list);
    return 0;
}
void unset_env(t_env *env_list, char *input)
{
    t_env *current = env_list;
    t_env *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->env->key, input) == 0)
        {
            if (prev == NULL)
                env_list = current->next;
            else
                prev->next = current->next;
            free(current->env->key);
            free(current->env->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
void add_envv_node(t_env **current, char *key, char *value) {
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    
    new_node->env = (env *)malloc(sizeof(env));
    new_node->env->key = ft_strdup(key);
    new_node->env->value = ft_strdup(value);
    
    new_node->next = NULL;
    
    (*current)->next = new_node;
    *current = new_node;
}
void add_env(t_env *env_list, char *input)
{
    char *key;
    char *value;
    t_env *current = env_list;
    key = ft_strndup(input, ft_strchr(input, '=') - input);
    value = ft_strdup(ft_strchr(input, '=') + 1);
    // while (current != NULL)
    // {
    //     if (strcmp(current->env->key, key) == 0)
    //     {
    //         free(current->env->value);
    //         current->env->value = value;
    //         free(key);
    //         return;
    //     }
    //     current = current->next;
    // }
    // current = env_list;
    // while (current->next != NULL)
    //     current = current->next;
    // current->next = malloc(sizeof(t_env));
    // current->next->env->key = key;
    // current->next->env->value = value;
    // current->next->next = NULL;
    add_envv_node(&current, key, value);
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


int execute_builtin(TokenNode *head, t_env *env_list) 
{
    TokenNode *current = head;

    if (strcmp(current->info.value, "echo") == 0)
        return ft_echo(current->next);
    else if (strcmp(current->info.value, "cd") == 0)
        return ft_cd(current->next, env_list);
    else if (strcmp(current->info.value, "pwd") == 0)
        return ft_pwd();
    else if (strcmp(current->info.value, "export") == 0)
        return ft_export(current->next, env_list);
    else if (strcmp(current->info.value, "unset") == 0)
        return ft_unset(current->next, env_list);
    else if (strcmp(current->info.value, "env") == 0)
        return ft_env(env_list);
    else if (strcmp(current->info.value, "exit") == 0)
        return ft_exit(current->next);
    return 0;
}
