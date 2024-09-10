#include "minishell.h"

void handle_heredoc(t_node *node, t_env **env_list)
{
    char *line;
    int fd;
    char *temp_file = "/tmp/minishell_heredoc";
    t_redirect *redirect = node->command->redirect;
    (void)env_list;

    while (redirect)
    {
        if (redirect->flag == 8) // Assuming 8 is the flag for heredoc
        {
            fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                return;
            }
            printf("Enter heredoc content. End with a line containing only \"%s\".\n", redirect->str);
            while (1)
            {
                line = readline("> ");
                if (!line || strcmp(line, redirect->str) == 0)
                {
                    free(line);
                    break;
                }
                dprintf(fd, "%s\n", line);
                free(line);
            }
            close(fd);

            // Fork and execute the command
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                // Child process
                fd = open(temp_file, O_RDONLY);
                if (fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);

                // Execute the command
                execvp(node->command->args[0], node->command->args);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else
            {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
                unlink(temp_file);  // Remove the temporary file
            }

            // We don't need to change the redirect flag or store the temp file name
            // because we've already executed the command
            break;  // Exit the loop after handling the heredoc
        }
        redirect = redirect->next;
    }
}



void handle_redirections(t_node *node)
{
    t_redirect *redirect = node->command->redirect;
    while (redirect)
    {
        int fd;
        if (redirect->flag == 4) // Output redirection '>'
        {
            fd = open(redirect->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        else if (redirect->flag == 5) // Append output redirection '>>'
        {
            fd = open(redirect->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        else if (redirect->flag == 3) // Input redirection '<'
        {
            fd = open(redirect->str, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        else if (redirect->flag == 8) // Heredoc '<<' (assuming flag 8 is for heredoc)
        {
            // Heredoc should be handled separately in handle_heredoc function
            // Here we just open the temporary file created by handle_heredoc
            fd = open(redirect->str, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        else
        {
            fprintf(stderr, "Unknown redirection type\n");
            exit(EXIT_FAILURE);
        }
        redirect = redirect->next;
    }
}