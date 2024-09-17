/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:53 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/17 02:34:50 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// NORM=OK!
void	execute_command_with_heredoc(t_node *temp, t_env **env_list,
	int *exit_status, const char *temp_file, MemoryManager *gc)
{
	int		status;
	int		fd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		fd = open_temp_file(temp_file, O_RDONLY);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
			if (temp->command && temp->command->args && temp->command->args[0])
				execute_single_command(temp, env_list, exit_status, gc);
		}
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}

void	handle_heredoc(t_node *node, t_env **env_list, int *exit_status, MemoryManager *gc)

{
	char		*temp_file;
	t_node		*temp;
	t_redirect	*redirect;

	(void)env_list;
	(void)exit_status;
	temp_file = create_temp_filename(gc);
	temp = node;
	while (temp)
	{
		redirect = temp->command->redirect;
		while (redirect)
		{
			if (redirect->flag == 8)
				handle_single_heredoc(redirect, temp_file,
					env_list, exit_status, gc);
			redirect = redirect->next;
		}
		if (temp->next == NULL)
			execute_command_with_heredoc(temp, env_list,
				exit_status, temp_file, gc);
		break ;
	}
	unlink(temp_file);
	free(temp_file);
}
