/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 02:18:09 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strndup(MemoryManager *manager, char *s, int n)
{
    char *result = (char *)gc_malloc(manager, n + 1);
    if (result == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}

char **set_env(MemoryManager *manager)
{
    char *pwd;
    char **envv;
    pwd = getcwd(NULL, 0);
    envv = (char **)gc_malloc(manager, 5 * sizeof(char *));
    envv[0] = ft_strjoin("PWD=", pwd, manager);
    envv[1] = ft_strdup(manager,"SHLVL=1");
    envv[2] = ft_strdup(manager,"_=/usr/bin/env");
    envv[3] = ft_strdup(manager, "OLDPWD");
    envv[4] = NULL;
    free(pwd);
    return envv;
}

void fill_env_list(MemoryManager *manager, char **envv, t_env *env_list)
{
    t_env *current;
    current = env_list;
    int i = 0;

    if (!envv[0])
        envv = set_env(manager);
    while (envv[i])
    {
        char **key_value = get_key_value(manager, envv[i]);

        current->env = (env *)gc_malloc(manager, sizeof(env));

        current->env->key = ft_strdup(manager, key_value[0]);
        current->env->value = ft_strdup(manager, key_value[1]);

        if (envv[i + 1])
        {
            current->next = (t_env *)gc_malloc(manager, sizeof(t_env));
            current = current->next;
        }
        else
            current->next = NULL;
        i++;
    }
}

char **get_key_value(MemoryManager *manager, char *var)
{
    char **key_value = (char **)gc_malloc(manager, 3 * sizeof(char *));
    if (!key_value)
    {
        perror("Memory allocation failed\n");
        exit(1);
    }

    if (ft_strchr(var, '=') == NULL)
    {
        key_value[0] = ft_strdup(manager, var);
        key_value[1] = NULL;
        key_value[2] = NULL;
        return key_value;
    }
    key_value[0] = ft_strndup(manager, var, ft_strchr(var, '=') - var);
    key_value[1] = ft_strdup(manager, ft_strchr(var, '=') + 1);
    key_value[2] = NULL;
    return key_value;
}
void print_env_list(t_env *env_list)
{
    
    t_env *current;
    current = env_list;
    while(current != NULL)
    {
        if (current->env->value != NULL || (current->env->value && current->env->value[0] == '\0'))
        printf("%s=%s\n", current->env->key, current->env->value);
        current = current->next;
       
    }
}