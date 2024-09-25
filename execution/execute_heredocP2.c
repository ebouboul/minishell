/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocP2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:53 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/25 14:14:55 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NORM=OK!
void	execute_command_with_heredoc(t_node *temp, const char *temp_file,
		t_exec_context *context)
{
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
				execute_single_command(temp, context->env_list,
					context->exit_status, context->gc);
		}
		my_exit(*context->exit_status, context->gc);
	}
	else
		ft_waitpid(pid, context->exit_status);
}

void	handle_heredoc(t_node *node, t_env **env_list, int *exit_status,
	t_MemoryManager *gc)
{
	char			*temp_file;
	t_node			*temp;
	t_exec_context	context;
	t_process_data	pdata;

	pdata.prev_pipe = -1;
	temp_file = create_temp_filename(gc);
	context.env_list = env_list;
	context.exit_status = exit_status;
	context.gc = gc;
	temp = node;
	while (temp)
	{
		handle_heredoc_redirects(temp, temp_file, &context);
		pdata.prev_pipe = handle_piping_and_forking(temp, &pdata,
				temp_file, &context);
		temp = temp->next;
	}
	while (wait(NULL) > 0)
		;
	unlink(temp_file);
	gc_free(gc, temp_file);
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
