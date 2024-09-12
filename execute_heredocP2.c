#include "minishell.h"

void execute_command_with_heredoc(t_node *temp, t_env **env_list, 
                                         int *exit_status, const char *temp_file)
{
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        int fd; 
        fd = open_temp_file(temp_file, O_RDONLY);
        if (fd != -1)
        {
            dup2(fd, STDIN_FILENO);
            close(fd);
            if (temp->command && temp->command->args && temp->command->args[0])
                execute_single_command(temp, env_list, exit_status);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}
void handle_heredoc(t_node *node, t_env **env_list, int *exit_status)
{
    char *temp_file;
    temp_file = create_temp_filename();
    t_node *temp; 
    temp = node;
    (void)env_list;
    (void)exit_status;

    while (temp)
    {
        t_redirect *redirect = temp->command->redirect;
        while (redirect)
        {
            if (redirect->flag == 8)
                handle_single_heredoc(redirect, temp_file);
            redirect = redirect->next;
        }
        if (temp->next == NULL)
            execute_command_with_heredoc(temp, env_list, exit_status, temp_file);
        break;
    }
    unlink(temp_file);
    free(temp_file);
}
