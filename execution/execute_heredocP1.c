/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:25 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/16 23:26:38 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**process_heredoc_input(const char *delimiter)
{
	char	*line;
	char	**args;
	int		i;

	i = 0;
	args = (char **)gc_malloc(sizeof(char *) * 100);
	signal(SIGINT, handler_c);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		args[i] = ft_strdup(line);
		free(line);
		i++;
	}
	args[i] = NULL;
	args = realloc(args, sizeof(char *) * (i + 1));
	return (args);
}

void	handle_single_heredoc(t_redirect *redirect, const char *temp_file,
		t_env **env_list, int *exit_status)
{
	int		fd;
	char	**args;
	char	*str;

	str = remove_all_quotes2(redirect->str);
	fd = open_temp_file(temp_file, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		args = process_heredoc_input(str);
		if (!ft_strchr(redirect->str, '"') && !ft_strchr(redirect->str, '\''))
			expan_herdoc(args, *env_list, *exit_status);
		while (args && *args)
		{
			write_to_file(fd, *args);
			args++;
		}
		close(fd);
	}
}