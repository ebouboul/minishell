/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:18 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/12 19:37:20 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_heredoc(t_node *node)
{
    if (!node || !node->command || !node->command->redirect)
        return 0;

    t_redirect *redirect; 
    redirect = node->command->redirect;
    while (redirect)
    {
        if (redirect->flag == 8)
            return 1;
        redirect = redirect->next;
    }
    return 0;
}
void execute_cmds(t_node *head, t_env **env_list, int *exit_status)
{
    t_node *current;
    current = head;
    while (current)
    {
            if (is_heredoc(current))
            {
                pid_t pid;
                pid = fork();
                if (pid == -1)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0 && is_heredoc(current))
                {
                    handle_heredoc(current, env_list, exit_status);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
            else if (current->next)
            {
                handle_pipe_and_multiple_commands(current, env_list, exit_status);
                break;
            }
            else
            {
                execute_single_command(current, env_list, exit_status);
            }
        current = current->next;
    }
}
int is_redirection(t_node *node)
{
    if (node == NULL || node->command == NULL || node->command->args == NULL || node->command->args[0] == NULL)
        return 0;
    return strcmp(node->command->args[0], ">") == 0 || strcmp(node->command->args[0], ">>") == 0 || strcmp(node->command->args[0], "<") == 0;
}
void execute_single_command(t_node *node, t_env **env_list, int *exit_status)
{
    if (node == NULL)
        return;
    
    char *cmd;
    char *executable_path;
    cmd = node->command->args[0];    
    if (cmd && is_builtin(cmd))
    {
        int stdout_copy;
        int stdin_copy;

        stdout_copy = dup(STDOUT_FILENO);
        stdin_copy = dup(STDIN_FILENO);
        handle_redirections(node, env_list, exit_status);
        *exit_status = execute_builtin(node, env_list);
        
        dup2(stdout_copy, STDOUT_FILENO);
        dup2(stdin_copy, STDIN_FILENO);
        close(stdout_copy);
        close(stdin_copy);
    }
    else
    {
        pid_t pid;
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            handle_redirections(node, env_list, exit_status);
            if (cmd)
            {
                executable_path = find_executable_in_path(cmd, *env_list);
                execve(executable_path, node->command->args, create_env_array(*env_list));
            }
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                *exit_status = WEXITSTATUS(status);
            else
                *exit_status = 1;
        }
    }
}
char *find_executable(const char *command, char **paths)
{
    int i = 0;
    char *full_path;

    while (paths[i] != NULL)
    {
        full_path = gc_malloc(strlen(paths[i]) + strlen(command) + 2);
        
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, command);
        if (access(full_path, X_OK) == 0)
            return full_path;

        gc_free(full_path);
        i++;
    }
    return NULL;
}
char **create_env_array(t_env *env_list)
{
    int count = 0;
    t_env *current;
    current = env_list;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    char **env_array;
    env_array = malloc(sizeof(char *) * (count + 1));
    current = env_list;
    int i = 0;
    while (current != NULL)
    {
        env_array[i] = malloc(strlen(current->env->key) + ft_strlen(current->env->value) + 2);
        sprintf(env_array[i], "%s=%s", current->env->key, current->env->value);
        i++;
        current = current->next;
    }
    env_array[i] = NULL;
    return env_array;
}
char **split_path(const char *path)
{
    char *path_copy;
    char **paths = NULL;
    int count;
    char *start;
    char *end;
    path_copy = strdup(path);
    count = 0;
    start = path_copy;
    end = path_copy;

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
