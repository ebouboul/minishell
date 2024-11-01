/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:25 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/27 21:57:37 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_temp_filename(t_MemoryManager *manager, int intdex)

{
	char	*tty;
	char	*base;

	tty = ttyname(STDIN_FILENO);
	base = "/tmp/minishell_heredoc_";
	ft_strjoin(base, tty + 9, manager);
	return (ft_strjoin(base, ft_itoa(intdex, manager), manager));
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

char	**process_heredoc_input(const char *delimiter, t_MemoryManager *gc)
{
	char	*line;
	char	**args;
	int		i;

	i = 0;
	args = (char **)gc_malloc(gc, sizeof(char *) * 100);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		args[i] = ft_strdup(gc, line);
		free(line);
		i++;
	}
	args[i] = NULL;
	args = gc_realloc(gc, args, sizeof(char *) * (i + 1));
	return (args);
}

void	handle_single_heredoc(t_redirect *redirect, const char *temp_file,
		t_exec_context *context)
{
	int		fd;
	char	**args;
	char	*str;

	str = remove_all_quotes2(redirect->str, context->gc);
	fd = open_temp_file(temp_file, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		args = process_heredoc_input(str, context->gc);
		if (!ft_strchr(redirect->str, '"') && !ft_strchr(redirect->str, '\''))
			expan_herdoc(args, *context->env_list, *context->exit_status,
				context->gc);
		while (args && *args)
		{
			write_to_file(fd, *args);
			args++;
		}
		close(fd);
	}
}
