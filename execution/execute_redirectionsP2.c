/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirectionsP2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:10 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/18 02:30:21 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
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
int ambigous_redirect(char *str)
{
	if (ft_strchr(str, ' ') || ft_strchr(str, '\t') || ft_strchr(str, '\n') 
	|| ft_strchr(str, '\v') || ft_strchr(str, '\f') || ft_strchr(str, '\r')
	|| ft_strlen(str) == 0)
	{
		print_error11(str, "ambiguous redirect");
		return (1);
	}
	return (0);
}

int	handle_redirections(t_node *node, t_env **env_list, int *exit_status)
{
	t_redirect	*redirect;

	redirect = node->command->redirect;
	(void)env_list;
	(void)exit_status;
	while (redirect)
	{
		if (redirect->flag != 8)
		{
			if(ambigous_redirect(redirect->str) == 0)
				handle_single_redirection(redirect);
			else
				return (1);
		}
		redirect = redirect->next;
	}
	return (0);
}

// void	wait_for_children(pid_t last_pid)
// {
// 	pid_t	wpid;
// 	int		status;

// 	wpid = waitpid(-1, &status, WUNTRACED);
// 	while (wpid > 0)
// 	{
// 		if (WIFEXITED(status))
// 		{
// 			printf("Child process %d exited with status %d\n",
// 				wpid, WEXITSTATUS(status));
// 		}
// 		else if (WIFSIGNALED(status))
// 		{
// 			printf("Child process %d killed by signal %d\n",
// 				wpid, WTERMSIG(status));
// 		}
// 	}
// 	if (wpid == -1 && errno != ECHILD)
// 	{
// 		perror("waitpid");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (last_pid > 0)
// 		waitpid(last_pid, &status, 0);
// }

void wait_for_children(pid_t last_pid, int *exit_status)
{
	int i;
    if (last_pid > 0)
    {
        int status;
        waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			*exit_status = 128 + WTERMSIG(status);
    }
	i = wait(NULL);
    while (i > 0)
		i = wait(NULL);	
}
