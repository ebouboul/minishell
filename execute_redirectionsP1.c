#include "minishell.h"

void handle_open_error(void)
{
    perror("open");
    exit(EXIT_FAILURE);
}
void handle_dup2_error(void)
{
    perror("dup2");
    exit(EXIT_FAILURE);
}
int open_file(const char *str, int flags)
{
    int fd = open(str, flags, 0644);
    if (fd == -1)
        handle_open_error();
    return fd;
}
void redirect_output(const char *str, int flags)
{
    int fd; 
    fd = open_file(str, flags);
    if (dup2(fd, STDOUT_FILENO) == -1)
        handle_dup2_error();
    close(fd);
}
void redirect_input(const char *str)
{
    int fd = open_file(str, O_RDONLY);
    if (dup2(fd, STDIN_FILENO) == -1)
        handle_dup2_error();
    close(fd);
}