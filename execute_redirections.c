#include "minishell.h"

void handle_heredoc(char *delimiter) // heredoc is a redirection from a file descriptor
{
    char *line;
    int fd;
    char *temp_file = "/tmp/minishell_heredoc";

    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }

    close(fd);

    fd = open(temp_file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    unlink(temp_file);
}

void handle_redirections(t_command *command)
{
    t_redirect *redirect = command->redirect;
	///
    int fd;

    while (redirect)
    {
        if (redirect->flag == TOKEN_REDIRECT_IN)
        {
            fd = open(redirect->str, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redirect->flag == TOKEN_REDIRECT_OUT)
        {
            fd = open(redirect->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redirect->flag == TOKEN_APPEND)
        {
            fd = open(redirect->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redirect->flag == TOKEN_HEREDOC)
        {
            handle_heredoc(redirect->str);
        }
        redirect = redirect->next;
    }
}