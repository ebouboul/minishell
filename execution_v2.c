#include "minishell.h"

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

    if (path_value == NULL)
    {
        fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
        return 127;
    }
    
    char **paths = split_path(path_value);
    char *executable_path = find_executable(command->args[0], paths);
    
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

void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list)
{
    t_node *current = head;
    int fd[2];
    int in = 0;
    while (current != NULL)
    {
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0)
        {
            dup2(in, 0);
            if (current->next != NULL)
                dup2(fd[1], 1);
            close(fd[0]);
            execute_single_command(current, env_list);
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
            close(fd[1]);
            in = fd[0];
            current = current->next;
        }
        else
        {
            perror("fork");
            return;
        }
    }
}