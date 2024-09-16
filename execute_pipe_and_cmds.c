/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_and_cmds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:13 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/16 17:43:51 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list, int *exit_status)
// {
//     t_node *current;
//     int fd[2];
//     int prev_pipe;
//     pid_t last_pid;
    
//     current = head;
//     prev_pipe = STDIN_FILENO;
//     last_pid = -1;
//     while (current != NULL)
//     {
//         if (pipe(fd) == -1)
//         {
//             perror("pipe");
//             return;
//         }

//         pid_t pid;
//         pid = fork();
//         if (pid == 0)
//         {
//             if (prev_pipe != STDIN_FILENO)
//             {
//                 dup2(prev_pipe, STDIN_FILENO);
//                 close(prev_pipe);
//             }

//             if (current->next != NULL)
//                 dup2(fd[1], STDOUT_FILENO);

//             close(fd[0]);
//             close(fd[1]);
//             execute_single_command(current, env_list, exit_status);
//             exit(0);
//         }

//         else if (pid > 0)
//         {
//             if (prev_pipe != STDIN_FILENO)
//                 close(prev_pipe);

//             close(fd[1]);

//             if (current->next == NULL)
//             {
//                 close(fd[0]);
//                 last_pid = pid;
//             }
//             else
//             {
//                 prev_pipe = fd[0];
//             }

//             current = current->next;
//         }
//         else
//         {
//             perror("fork");
//             return;
//         }
//     }

//     if (last_pid > 0)
//     {
//         int status;
//         waitpid(last_pid, &status, 0);
//     }

//     while (wait(NULL) > 0);
// }

void setup_pipe(int fd[2])
{
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void handle_child_io(int prev_pipe, int fd[2], t_node *current)
{
    if (prev_pipe != STDIN_FILENO)
    {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }
    if (current->next != NULL)
        dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
}

void handle_child_process(int prev_pipe, int fd[2], t_node *current, t_env **env_list, int *exit_status)
{
    handle_child_io(prev_pipe, fd, current);
    execute_single_command(current, env_list, exit_status);
    exit(0);
}

void handle_parent_io(int *prev_pipe, int fd[2], t_node *current, pid_t *last_pid, pid_t pid)
{
    if (*prev_pipe != STDIN_FILENO)
        close(*prev_pipe);
    close(fd[1]);
    if (current->next == NULL)
    {
        close(fd[0]);
        *last_pid = pid;
    }
    else
    {
        *prev_pipe = fd[0];
    }
}

void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list, int *exit_status)
{
    t_node *current = head;
    int fd[2], prev_pipe = STDIN_FILENO;
    pid_t last_pid = -1, pid;

    while (current != NULL)
    {
        setup_pipe(fd);
        pid = fork();
        if (pid == 0)
            handle_child_process(prev_pipe, fd, current, env_list, exit_status);
        else if (pid > 0)
        {
            handle_parent_io(&prev_pipe, fd, current, &last_pid, pid);
            current = current->next;
        }
        else
        {
            perror("fork");
            return;
        }
    }
    wait_for_children(last_pid);
}
