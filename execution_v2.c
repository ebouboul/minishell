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

int if_file_has_permission(char *file)
{
    struct stat file_stat;

    if (stat(file, &file_stat) != 0)
    {
        perror("stat");
        return 0;
    }

    if (S_ISDIR(file_stat.st_mode))
    {
        fprintf(stderr, "minishell: %s: This is a directory\n", file);
        return 0;
    }

    if (access(file, F_OK) == 0)
    {
        if (access(file, R_OK) == 0)
        {
            if (access(file, W_OK) == 0)
            {
                if (access(file, X_OK) == 0)
                    return 1;
            }
        }
    }
    return 0;
}
char *find_executable_in_path(char *command, t_env *env_list)
{
    char *path_value = gett_env_value("PATH", env_list);
    if (path_value == NULL)
        return NULL;

    char **paths = split_path(path_value);
    char *executable_path = find_executable(command, paths);

    int i = 0;
    while (paths && paths[i] != NULL)
    {
        free(paths[i]);
        i++;
    }
    free(paths);

    return executable_path;
}

int check_file_permissions(char *file)
{
    struct stat file_stat;
    if (stat(file, &file_stat) != 0)
        return -1;

    if (S_ISDIR(file_stat.st_mode))
    {
        fprintf(stderr, "minishell: %s: is a directory\n", file);
        return 126;
    }

    if (access(file, X_OK) != 0)
    {
        fprintf(stderr, "minishell: %s: Permission denied\n", file);
        return 126;
    }

    return 0;
}

int execute_command(char *executable_path, char **args, t_env *env_list)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        char **env_array = create_env_array(env_list);
        execve(executable_path, args, env_array);
        perror("execve");
        exit(1);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return 1;
    }
    else
    {
        perror("fork");
        return 1;
    }
}

int execute_external(t_command *command, t_env *env_list)
{
    char *executable_path = find_executable_in_path(command->args[0], env_list);

    if (executable_path == NULL)
    {
        int check_result = check_file_permissions(command->args[0]);
        if (check_result == 0)
            executable_path = strdup(command->args[0]);
        else if (check_result > 0)
            return check_result;
    }

    if (executable_path == NULL)
    {
        fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
        return 127;
    }

    int result = execute_command(executable_path, command->args, env_list);
    free(executable_path);
    return result;
}
// void hadle_pipes(int fd[2], int out, t_node *current)
// {
//     if (out == -1)
//     {
//         dup2(fd[PIPE_INPUT], STDOUT_FILENO);
//            close(fd[PIPE_OUTPUT]);
//            close(fd[PIPE_INPUT]);
//     }
//     else if (out != -1 && current->next != NULL)
//       {
//             dup2(out, STDIN_FILENO);
//             dup2(fd[PIPE_INPUT], STDOUT_FILENO);
//             close(fd[PIPE_INPUT]);
//             close(fd[PIPE_OUTPUT]);
//             close(out);
//      }
//     else
//      {
//             dup2(out, STDIN_FILENO);
//             close(out);
//     }
// }

void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list)
{
    t_node *current = head;
    int fd[2];
    int prev_pipe = STDIN_FILENO;
    pid_t last_pid = -1;

    while (current != NULL)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            return;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            if (prev_pipe != STDIN_FILENO)
            {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            if (current->next != NULL)
            {
                dup2(fd[1], STDOUT_FILENO);
            }

            close(fd[0]);
            close(fd[1]);

            execute_single_command(current, env_list);
            exit(0);
        }
        else if (pid > 0)
        {
            // Parent process
            if (prev_pipe != STDIN_FILENO)
                close(prev_pipe);

            close(fd[1]);

            if (current->next == NULL)
            {
                // This is the last command in the pipeline
                close(fd[0]);
                last_pid = pid;
            }
            else
            {
                prev_pipe = fd[0];
            }

            current = current->next;
        }
        else
        {
            perror("fork");
            return;
        }
    }

    // Wait only for the last process in the pipeline
    if (last_pid > 0)
    {
        int status;
        waitpid(last_pid, &status, 0);
    }

    // Clean up any remaining child processes
    while (wait(NULL) > 0);
}
