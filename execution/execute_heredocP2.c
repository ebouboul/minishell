/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:53 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/27 21:57:29 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NORM=OK!
void	unlink_all(char **files, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		unlink(files[j]);
		j++;
	}
}

void	handle_heredoc_redirects(t_node *node, char *temp_file,
		t_exec_context *context)
{
	t_redirect	*redirect;

	redirect = node->command->redirect;
	while (redirect)
	{
		if (redirect->flag == 8)
			handle_single_heredoc(redirect, temp_file, context);
		redirect = redirect->next;
	}
}

char	**handle_heredoc(t_node *node, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	char			**files;
	t_node			*temp;
	t_exec_context	context;
	int				i;

	context.env_list = env_list;
	context.exit_status = exit_status;
	context.gc = gc;
	temp = node;
	i = -1;
	files = gc_malloc(gc, sizeof(char *) * 100);
	while (temp)
	{
		if (is_herefoc1(temp))
		{
			i++;
			files[i] = ft_strdup(gc, create_temp_filename(gc, i));
			handle_heredoc_redirects(temp, files[i], &context);
		}
		temp = temp->next;
	}
	return (files);
}

void	ft_waitpid(pid_t last_pid, int *exit_status)
{
	int	status;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
}
