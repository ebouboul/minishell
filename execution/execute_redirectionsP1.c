/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirectionsP1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:03 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/21 04:19:21 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NORM=OK!
int	handle_open_error(void)
{
	perror("open");
	return (EXIT_FAILURE);
}

int	handle_dup2_error(void)
{
	perror("dup2");
	return (EXIT_FAILURE);
}

int	open_file(const char *str, int flags)
{
	int	fd;

	fd = open(str, flags, 0644);
	if (fd == -1)
		return (handle_open_error());
	return (fd);
}

int	redirect_output(const char *str, int flags)
{
	int	fd;

	fd = open_file(str, flags);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (handle_dup2_error());
	close(fd);
	return (EXIT_SUCCESS);
}

int	redirect_input(const char *str)
{
	int	fd;

	fd = open_file(str, O_RDONLY);
	if (fd == -1)
		return (EXIT_FAILURE);
	else if (dup2(fd, STDIN_FILENO) == -1)
		return (handle_dup2_error());
	close(fd);
	return (EXIT_SUCCESS);
}
