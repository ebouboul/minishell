/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/29 03:43:20 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    int i = 1;
    t_command *current = command;
    while (current->args[i] != NULL)
    {
        if (current->args[i] != NULL && strcmp(current->args[i], "-n") == 0)
        {
            if (process_echo_option(current->args[i], &option))
                break;
        }
        else
        {
            printf("%s ", current->args[i]);
        }
        i++;
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
void replace_env_value(t_env *env_list, char *key, char *value)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (strcmp(current->env->key, key) == 0)
        {
            gc_free(current->env->value);
            current->env->value = ft_strdup(value);
            break;
        }
        current = current->next;
    }
}
int ft_cd(t_command *command, t_env **env_list)
{
    char *path;
    t_command *current = command;
    t_env *current_env = *env_list;
    if (current->args[1] == NULL)
    {
        path = get_env_value(*env_list, "HOME");
        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    }
    else
    {
        path = current->args[1];
    }
    char *oldpwd = getcwd(NULL, 0);
    int status = chdir(path);
    if(status != -1)
    {
        char *pwd = getcwd(NULL, 0);
        replace_env_value(current_env, "OLDPWD", oldpwd);
        replace_env_value(current_env, "PWD", pwd);
        free(oldpwd);
        free(pwd);
    }
    else
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
    int status = 0;
    if (command->args[1] != NULL)
    {
        status = ft_atoi(command->args[1]);
    }
    gc_free_all();
    exit(status);
    return 0;
}
int check_key_from_env(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (strcmp(current->env->key, key) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
void add_env_node(t_env **current, char *key, char *value)
{

    t_env *new_node = (t_env *)gc_malloc(sizeof(t_env));
    new_node->env = (env *)gc_malloc(sizeof(env));
    new_node->env->key = ft_strdup((const char *)key);
    new_node->env->value = ft_strdup((const char *)value);
    new_node->next = NULL;
    // printf("key: %s\n", key);
    // (*current)->next = new_node;
    *current = new_node;
    // printf("%s\n", (*current)->env->key);
}

char **get_key_value_for_plus(char *var)
{
    char **key_value = (char **)gc_malloc(3 * sizeof(char *));
    if (key_value == NULL)
    {
        perror("Memory allocation failed\n");
        exit(1);
    }
    key_value[0] = ft_strndup(var, ft_strchr(var, '+') - var);
    key_value[1] = ft_strdup(ft_strchr(var, '+') + 2);
    key_value[2] = NULL;
    return key_value;
}


int ft_export(t_command *command, t_env **env_list)
{
    t_command *current = command;
    t_env *current_env = *env_list; // Reset current_env to start of the list
    char *plus;
    
    while (current != NULL)
    {

        if (current->args[1] == NULL)
        {
            // Print all environment variables
            while (current_env != NULL)
            {
                printf("declare -x %s=\"%s\"\n", current_env->env->key, current_env->env->value);
                current_env = current_env->next;
            }
            return 0;
        }
        // else if((ft_strchr(current->args[1], '+'))[1] == '=')
        
        else
        {
            // Parse the key-value pair
            char **key_value = get_key_value(current->args[1]);
            
            if (key_value[1] == NULL)
            {
                // Case: Only a key is provided, check if it exists in env_list
                if (check_key_from_env(*env_list, key_value[0]))
                {
                    while (current_env != NULL)
                    {
                        if (strcmp(current_env->env->key, key_value[0]) == 0)
                        {
                            printf("declare -x %s=\"%s\"\n", current_env->env->key, current_env->env->value);
                            break;
                        }
                        current_env = current_env->next;
                    }
                }
            }
            else if((plus = ft_strchr(current->args[1], '+')) && plus[1] == '=')
            {
                char **key_value_plus = get_key_value_for_plus(current->args[1]);
                if (check_key_from_env(*env_list, key_value_plus[0]))
                {
                    while (current_env != NULL)
                    {
                        if (strcmp(current_env->env->key, key_value_plus[0]) == 0)
                        {
                            char *new_value = ft_strjoin(current_env->env->value, key_value_plus[1]);
                            gc_free(current_env->env->value);
                            current_env->env->value = new_value;
                            break;
                        }
                        current_env = current_env->next;
                    }
                }
            }
            else
            {
                // Case: Key and value are provided
                if (check_key_from_env(*env_list, key_value[0]))
                {
                    // Update the value if the key exists
                    while (current_env != NULL)
                    {
                        if (strcmp(current_env->env->key, key_value[0]) == 0)
                        {
                            gc_free(current_env->env->value);
                            current_env->env->value = ft_strdup(key_value[1]);
                            break;
                        }
                        current_env = current_env->next;
                    }
                }
                else
                {
                    // Find the last node in the list
                    while (current_env->next != NULL)
                    {
                        current_env = current_env->next;
                    }
                    current_env->next = (t_env *)gc_malloc(sizeof(t_env));
                    current_env->next->env = (env *)gc_malloc(sizeof(env));
                    current_env->next->env->key = ft_strdup(key_value[0]);
                    current_env->next->env->value = ft_strdup(key_value[1]);
                    current_env->next->next = NULL;
                    
                    //print_env_list(*env_list);
                    
                }
            }
            
            gc_free(key_value);
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
                        gc_free(current_env->env->key);
                        gc_free(current_env->env->value);
                        gc_free(current_env->env);
                        gc_free(current_env);
                        current_env = *env_list;
                        break;
                    }
                    prev->next = current_env->next;
                    gc_free(current_env->env->key);
                    gc_free(current_env->env->value);
                    gc_free(current_env->env);
                    gc_free(current_env);
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
        status = ft_cd(current->command, env_list);
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
