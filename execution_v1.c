/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:18 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/13 17:18:45 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// NORM=OK!
int	is_heredoc(t_node *node)
{
	t_redirect	*redirect;

	if (!node || !node->command || !node->command->redirect)
		return (0);
	redirect = node->command->redirect;
	while (redirect)
	{
		if (redirect->flag == 8)
			return (1);
		redirect = redirect->next;
	}
	return (0);
}

void	execute_heredoc(t_node *current, t_env **env_list, int *exit_status)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		handle_heredoc(current, env_list, exit_status);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(pid, &status, 0);
}

void	execute_piped_commands(t_node *current,
		t_env **env_list, int *exit_status)
{
	handle_pipe_and_multiple_commands(current, env_list, exit_status);
}

void	execute_cmds(t_node *head, t_env **env_list, int *exit_status)
{
	t_node	*current;

	current = head;
	while (current)
	{
		if (is_heredoc(current))
			execute_heredoc(current, env_list, exit_status);
		else if (current->next)
		{
			execute_piped_commands(current, env_list, exit_status);
			break ;
		}
		else
		{
			execute_single_command(current, env_list, exit_status);
		}
		current = current->next;
	}
}

int	is_redirection(t_node *node)
{
	if (node == NULL || node->command == NULL
		|| node->command->args == NULL || node->command->args[0] == NULL)
		return (0);
	return (strcmp(node->command->args[0], ">") == 0
		|| strcmp(node->command->args[0], ">>") == 0
		|| strcmp(node->command->args[0], "<") == 0);
}
