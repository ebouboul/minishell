/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_and_cmds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:13 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/12 19:36:15 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list, int *exit_status)
{
    t_node *current;
    int fd[2];
    int prev_pipe;
    pid_t last_pid;
    
    current = head;
    prev_pipe = STDIN_FILENO;
    last_pid = -1;
    while (current != NULL)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            return;
        }

        pid_t pid;
        pid = fork();
        if (pid == 0)
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
            execute_single_command(current, env_list, exit_status);
            exit(0);
        }

        else if (pid > 0)
        {
            if (prev_pipe != STDIN_FILENO)
                close(prev_pipe);

            close(fd[1]);

            if (current->next == NULL)
            {
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

    if (last_pid > 0)
    {
        int status;
        waitpid(last_pid, &status, 0);
    }

    while (wait(NULL) > 0);
}
