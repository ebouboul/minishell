/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirectionsP2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:10 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/16 18:27:36 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// NORM=OK!
void	handle_single_redirection(t_redirect *redirect)
{
	if (redirect->flag == 4)
		redirect_output(redirect->str, O_WRONLY | O_CREAT | O_TRUNC);
	else if (redirect->flag == 5)
		redirect_output(redirect->str, O_WRONLY | O_CREAT | O_APPEND);
	else if (redirect->flag == 3)
		redirect_input(redirect->str);
	else
	{
		fprintf(stderr, "Unknown redirection type\n");
		exit(EXIT_FAILURE);
	}
}

void	handle_redirections(t_node *node, t_env **env_list, int *exit_status)
{
	t_redirect	*redirect;

	redirect = node->command->redirect;
	(void)env_list;
	(void)exit_status;
	while (redirect)
	{
		if (redirect->flag != 8)
			handle_single_redirection(redirect);
		redirect = redirect->next;
	}
}

void	wait_for_children(pid_t last_pid)
{
	pid_t	wpid;
	int		status;

	wpid = waitpid(-1, &status, WUNTRACED);
	while (wpid > 0)
	{
		if (WIFEXITED(status))
		{
			printf("Child process %d exited with status %d\n",
				wpid, WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			printf("Child process %d killed by signal %d\n",
				wpid, WTERMSIG(status));
		}
	}
	if (wpid == -1 && errno != ECHILD)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (last_pid > 0)
		waitpid(last_pid, &status, 0);
}
