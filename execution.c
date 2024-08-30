#include "minishell.h"

void execute_commands(t_node *head, t_env **env_list)
{
    t_node *current = head;
    while (current != NULL)
    {
        execute_single_command(current, env_list);
        current = current->next;
    }
}
void execute_single_command(t_node *node, t_env **env_list)
{
    if (node == NULL || node->command == NULL || node->command->args == NULL || node->command->args[0] == NULL)
        return;
    char *cmd = node->command->args[0];
    if (is_builtin(cmd))
    {
        node->exit_status = execute_builtin(node, env_list);
    }
    else
    {
        // printf("DEBUG: Executing external command: %s\n", cmd);
        node->exit_status = execute_external(node->command, *env_list);
    }
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
        {
            return full_path;
        }

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
char *gett_env_value(const char *key, t_env *env_list)
{
    while (env_list != NULL)
    {
        if (strcmp(env_list->env->key, key) == 0)
            return env_list->env->value;
        env_list = env_list->next;
    }
    return NULL;
}

int execute_external(t_command *command, t_env *env_list)
{
    char *path_value = gett_env_value("PATH", env_list);
    // printf("DEBUG: PATH value: %s\n", path_value);
    
    if (path_value == NULL)
    {
        fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
        return 127;
    }
    
    char **paths = split_path(path_value);
    char *executable_path = find_executable(command->args[0], paths);
    // printf("DEBUG: Executable path found: %s\n", executable_path);
    
    if (executable_path == NULL)
    {
        fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
        int i = 0;
        while (paths[i] != NULL)
        {
            free(paths[i]);
            i++;
        }
        free(paths);
        return 127;
    }
    
    pid_t pid = fork();
    if (pid == 0)
    {
        char **env_array = create_env_array(env_list);
        // printf("DEBUG: Executing command: %s\n", executable_path);
        execve(executable_path, command->args, env_array);
        perror("execve");
        exit(1);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        free(executable_path);
        int i = 0;
        while (paths[i] != NULL)
        {
            free(paths[i]);
            i++;
        }
        free(paths);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return 1;
    }
    else
    {
        perror("fork");
        free(executable_path);
        int i = 0;
        while (paths[i] != NULL)
        {
            free(paths[i]);
            i++;
        }
        free(paths);
        return 1;
    }
}