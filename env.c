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
// void fill_env_list(char **envv, t_env *env_list) 
// {
//     t_env *current = env_list;
//     int i = 0;
//     while(envv[i])
//     {
//         char **key_value = get_key_value(envv[i]);
//         current->env = (env *)malloc(sizeof(env));
//         current->env->key = ft_strdup(key_value[0]);
//         current->env->value = ft_strdup(key_value[1]);
//         current->next = (t_env *)malloc(sizeof(t_env));
//         current = current->next;
//         // current->next = NULL;

//         i++;
        
//     }
//     current = NULL; 
// }


void fill_env_list(char **envv, t_env *env_list) 
{
    t_env *current = env_list;
    int i = 0;

    while (envv[i])
    {
        char **key_value = get_key_value(envv[i]);

        // Allocate memory for the current environment variable
        current->env = (env *)malloc(sizeof(env));
        if (current->env == NULL)
        {
            // Handle malloc failure
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        // Set key and value
        current->env->key = ft_strdup(key_value[0]);
        current->env->value = ft_strdup(key_value[1]);

        // Allocate memory for the next node and prepare it
        if (envv[i + 1]) // Allocate a new node only if there are more environment variables to process
        {
            current->next = (t_env *)malloc(sizeof(t_env));
            if (current->next == NULL)
            {
                perror("malloc failed");
                exit(EXIT_FAILURE);
            }
            current = current->next;
        }
        else
        {
            current->next = NULL; // Set the next pointer of the last node to NULL
        }

        i++;
        free(key_value); // Clean up key_value after use
    }
}

void print_env_list(t_env *env_list)
{
    
    t_env *current = env_list;
    // current->index = 0;
    while(current != NULL)
    {
        printf("%s=%s\n", current->env->key, current->env->value);
        // current->index++;
        current = current->next;
       
    }
}

char **get_key_value(char *var)
{
    char **key_value = (char**)malloc(3 * sizeof(char*));

    if (key_value == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    key_value[0] = ft_strndup(var, ft_strchr(var, '=') - var);
    key_value[1] = ft_strdup(ft_strchr(var, '=') + 1);
    key_value[2] = NULL;
    return key_value;
    
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


// int check_key_from_env(t_env *env_list, char *key)
// {
//     t_env *current = env_list;
//     while (current != NULL) {
//         if (strcmp(current->env->key, key) == 0)
//         {
//             printf("%s already exists\n", key);
//             return 0;
//         }
//         current = current->next;
//     }
//     return 1;
// }

int check_key_if_deja(t_env *env_list, char *var)
{
    t_env *current = env_list;
    char *key = strtok(var, "=");
    return check_key_from_env(current, key);
}




