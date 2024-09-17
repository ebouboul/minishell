/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:57:40 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 20:11:05 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_key_from_env(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
char *get_env_value(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
            return current->env->value;
        current = current->next;
    }
    return NULL;
}

void replace_env_value(t_env *env_list, char *key, char *value, MemoryManager *manager)
{
    t_env *current = env_list;
    if (value == NULL)
        return;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
        {
            gc_free(manager, current->env->value);
            current->env->value = ft_strdup(manager, value);
            break;
        }
        current = current->next;
    }
}


int ft_pwd()
{
    char *path = getcwd(NULL, 0);
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