#include "minishell.h"

void execute_commands(t_node *head, t_env **env_list)
{
    t_node *current = head;
    while (current != NULL)
    {
        if(current->next != NULL)
        {
            handle_pipe_and_multiple_commands(current, env_list);
            return;
        }
        else
        {
            execute_single_command(current, env_list);
            current = current->next;
        }
    }
}


void execute_single_command(t_node *node, t_env **env_list)
{
    if (node == NULL || node->command == NULL || node->command->args == NULL || node->command->args[0] == NULL)
        return;
    char *cmd = node->command->args[0];
    if (is_builtin(cmd))
        node->exit_status = execute_builtin(node, env_list);
    else
        node->exit_status = execute_external(node->command, *env_list);
}
char *find_executable(const char *command, char **paths)
{
    int i = 0;
    char *full_path;
    

    while (paths[i] != NULL)
    {
        full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
        
        sprintf(full_path, "%s/%s", paths[i], command);
        if (access(full_path, X_OK) == 0)
            return full_path;

        free(full_path);
        i++;
    }


    return NULL;
}

char **create_env_array(t_env *env_list)
{
    int count = 0;
    t_env *current = env_list;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    char **env_array = malloc(sizeof(char *) * (count + 1));
    current = env_list;
    int i = 0;
    while (current != NULL)
    {
        env_array[i] = malloc(strlen(current->env->key) + strlen(current->env->value) + 2);
        sprintf(env_array[i], "%s=%s", current->env->key, current->env->value);
        i++;
        current = current->next;
    }
    env_array[i] = NULL;
    return env_array;
}

char **split_path(const char *path)
{
    char *path_copy = strdup(path);
    char **paths = NULL;
    int count = 0;
    char *start = path_copy;
    char *end = path_copy;

    while (*end != '\0')
    {
        if (*end == ':')
        {
            *end = '\0';
            paths = realloc(paths, sizeof(char *) * (count + 1));
            paths[count++] = strdup(start);
            start = end + 1;
        }
        end++;
    }
    if (start != end)
    {
        paths = realloc(paths, sizeof(char *) * (count + 1));
        paths[count++] = strdup(start);
    }
    paths = realloc(paths, sizeof(char *) * (count + 1));
    paths[count] = NULL;
    free(path_copy);
    return paths;
}
