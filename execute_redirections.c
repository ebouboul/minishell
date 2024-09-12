#include "minishell.h"

// void handle_heredoc(t_node *node, t_env **env_list, int *exit_status)
// {
//     char *line;
//     int fd;
//     char *tty = ttyname(STDIN_FILENO);
//     char *temp_file = "/tmp/minishell_heredoc_";
//     temp_file = ft_strjoin(temp_file, tty+9);
//     t_redirect *redirect = node->command->redirect;
//     (void)env_list;
//     t_node *temp = node;



// while (temp)
// {
//         while (redirect)
//             {
//                 if (redirect->flag == 8) // Assuming 8 is the flag for heredoc
//                 {

//                     fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                     if (fd == -1)
//                     {
//                         perror("open");
//                         return;
//                     }
//                     signal(SIGINT, handler_c);

//                     while (1)
//                     {
//                         line = readline("> ");
//                         if (!line || strcmp(line, redirect->str) == 0)
//                         {
//                             free(line);
//                             break;
//                         }
//                         write(fd, line, strlen(line));
//                         write(fd, "\n", 1);
//                         free(line);
//                     }
//                     close(fd);
//                 }
//                 redirect = redirect->next;
//             }

//                 // Fork and execute the command
//                if (temp->next == NULL)
//                {
//                 pid_t pid = fork();
//                 if (pid == -1)
//                 {
//                     perror("fork");
//                     return;
//                 }
//                 else if (pid == 0)
//                 {
//                     // Child process
//                     fd = open(temp_file, O_RDONLY);
//                     if (fd == -1)
//                     {
//                         perror("open");
//                         exit(EXIT_FAILURE);
//                     }
//                     dup2(fd, STDIN_FILENO);
//                     close(fd);

//                     if (temp->command && temp->command->args && temp->command->args[0])
//                     execute_single_command(temp, env_list, exit_status);
//                     // perror("execvp");
//                     exit(EXIT_FAILURE);
//                 }
//                 else
//                 {
//                     // Parent process
//                     int status;
//                     waitpid(pid, &status, 0);
//                 }


//                 // We don't need to change the redirect flag or store the temp file name
//                 // because we've already executed the command
//                 // Exit the loop after handling the heredoc
//                }
//                 break; 

//         }
//             unlink(temp_file); 
//              // Remove the temporary file

//         temp = temp->next;

// }

// void handle_redirections(t_node *node)
// {
//     t_redirect *redirect = node->command->redirect;
//     while (redirect)
//     {
//         fprintf(stderr, "Debug: Handling redirection with flag %d and string %s\n", redirect->flag, redirect->str);
        
//         int fd;
//         if (redirect->flag == 4) // Output redirection '>'
//         {
//             fd = open(redirect->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd == -1)
//             {
//                 perror("open");
//                 exit(EXIT_FAILURE);
//             }
//             fprintf(stderr, "Debug: Redirecting stdout to file %s (fd: %d)\n", redirect->str, fd);
//             if (dup2(fd, STDOUT_FILENO) == -1)
//             {
//                 perror("dup2");
//                 exit(EXIT_FAILURE);
//             }
//             fprintf(stderr, "Debug: dup2 successful\n");
//             close(fd);
//         }
//         else if (redirect->flag == 5) // Append output redirection '>>'
//         {
//             fd = open(redirect->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd == -1)
//             {
//                 perror("open");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDOUT_FILENO) == -1)
//             {
//                 perror("dup2");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         else if (redirect->flag == 3) // Input redirection '<'
//         {
//             fd = open(redirect->str, O_RDONLY);
//             if (fd == -1)
//             {
//                 perror("open");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDIN_FILENO) == -1)
//             {
//                 perror("dup2");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         else if (redirect->flag == 8) // Heredoc '<<' (assuming flag 8 is for heredoc)
//         {
//             // Heredoc should be handled separately in handle_heredoc function
//             // Here we just open the temporary file created by handle_heredoc
//             fd = open(redirect->str, O_RDONLY);
//             if (fd == -1)
//             {
//                 perror("open");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDIN_FILENO) == -1)
//             {
//                 perror("dup2");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         else
//         {
//             fprintf(stderr, "Unknown redirection type\n");
//             exit(EXIT_FAILURE);
//         }
//         redirect = redirect->next;
//     }
//         fprintf(stderr, "Debug: Finished handling redirections\n");
// }

void handle_heredoc(t_node *node, t_env **env_list, int *exit_status)
{
    (void)exit_status;
    char *tty = ttyname(STDIN_FILENO);
    char *temp_file = ft_strjoin("/tmp/minishell_heredoc_", tty + 9);
    t_redirect *redirect = node->command->redirect;
    (void)env_list;
    t_node *temp = node;

    while (temp)
    {
        while (redirect)
        {
            if (redirect->flag == 8) // Assuming 8 is the flag for heredoc
            {
                int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open");
                    return;
                }
                signal(SIGINT, handler_c);

                char *line;
                while (1)
                {
                    line = readline("> ");
                    if (!line || strcmp(line, redirect->str) == 0)
                    {
                        free(line);
                        break;
                    }
                    write(fd, line, strlen(line));
                    write(fd, "\n", 1);
                    free(line);
                }
                close(fd);

                // Store the temp file name in the redirect structure
                redirect->str = strdup(temp_file);
            }
            redirect = redirect->next;
        }

        // We don't need to fork and execute the command here
        // This will be handled by execute_single_command
        temp = temp->next;
    }

    free(temp_file);
}

void handle_redirections(t_node *node)
{
    t_redirect *redirect = node->command->redirect;
    while (redirect)
    {
        fprintf(stderr, "Debug: Handling redirection with flag %d and string %s\n", redirect->flag, redirect->str);
        
        int fd;
        switch (redirect->flag)
        {
            case 4: // Output redirection '>'
                fd = open(redirect->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                fprintf(stderr, "Debug: Redirecting stdout to file %s (fd: %d)\n", redirect->str, fd);
                if (dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                fprintf(stderr, "Debug: dup2 successful\n");
                close(fd);
                break;

            case 5: // Append output redirection '>>'
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
                break;

            case 3: // Input redirection '<'
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
                break;

            case 8: // Heredoc '<<'
                if (redirect->flag)
                {
                    fd = open(redirect->str, O_RDONLY);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    // fprintf(stderr, "Debug: Redirecting stdin from heredoc file %s (fd: %d)\n", redirect->heredoc_file, fd);
                    if (dup2(fd, STDIN_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    close(fd);
                }
                else
                {
                    fprintf(stderr, "Error: Heredoc file not set up\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                fprintf(stderr, "Unknown redirection type\n");
                exit(EXIT_FAILURE);
        }
        redirect = redirect->next;
    }
    fprintf(stderr, "Debug: Finished handling redirections\n");
}