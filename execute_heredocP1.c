/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:25 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/12 19:57:48 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 
// NORM = OK
char	*create_temp_filename(void)
{
	char	*tty;
	char	*base;

	tty = ttyname(STDIN_FILENO);
	base = "/tmp/minishell_heredoc_";
	return (ft_strjoin(base, tty + 9));
}

int	open_temp_file(const char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd == -1)
		perror("open");
	return (fd);
}

void	write_to_file(int fd, const char *str)
{
	write(fd, str, strlen(str));
	write(fd, "\n", 1);
}

void	process_heredoc_input(int fd, const char *delimiter)
{
	char	*line;

	signal(SIGINT, handler_c);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_to_file(fd, line);
		free(line);
	}
}

void	handle_single_heredoc(t_redirect *redirect, const char *temp_file)
{
	int	fd;

	fd = open_temp_file(temp_file, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		process_heredoc_input(fd, redirect->str);
		close(fd);
	}
}
