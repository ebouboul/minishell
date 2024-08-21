#include "minishell.h"

char *ft_strndup(char *s, int n)
{
    char *result = (char*)malloc(n + 1);
    if (result == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}

char **get_key_value(char *var)
{
    char **key_value = (char**)malloc(2 * sizeof(char*));

    if (key_value == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    key_value[0] = ft_strndup(var, ft_strchr(var, '=') - var);
    key_value[1] = ft_strdup(ft_strchr(var, '=') + 1);
    return key_value;
    
}



void add_env_node(t_env **current, char *key, char *value) {
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    
    new_node->env = (env *)malloc(sizeof(env));
    new_node->env->key = ft_strdup(key);
    new_node->env->value = ft_strdup(value);
    
    new_node->next = NULL;
    
    (*current)->next = new_node;
    *current = new_node;
}

void fill_env_list(char **env, t_env *env_list) {
    int i = 0;
    
    t_env *current = env_list;
    
    char **key_value = get_key_value(env[i]);
    env_list->env = malloc(sizeof(env));
    env_list->env->key = ft_strdup(key_value[0]);
    env_list->env->value = ft_strdup(key_value[1]);
    env_list->next = NULL;

    i++;

    while (env[i]) {
        key_value = get_key_value(env[i]);
        add_env_node(&current, key_value[0], key_value[1]);
        i++;
    }
}

void print_env_list(t_env *env_list)
{
    
    t_env *current = env_list;
    // current->index = 0;
    while(current->next != NULL)
    {
        printf("%s=%s\n", current->env->key, current->env->value);
        // current->index++;
        current = current->next;
       
    }
}
int check_key_from_env(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL) {
        if (strcmp(current->env->key, key) == 0)
        {
            printf("%s already exists\n", key);
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int check_key_if_deja(t_env *env_list, char *var)
{
    t_env *current = env_list;
    char *key = strtok(var, "=");
    return check_key_from_env(current, key);
}




