/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_and_cmds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 00:39:32 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:32:25 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_pipe(t_process_data *proc_data)
{
	if (pipe(proc_data->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	handle_child_io(t_process_data *proc_data, t_node *current)
{
	if (proc_data->prev_pipe != STDIN_FILENO)
	{
		dup2(proc_data->prev_pipe, STDIN_FILENO);
		close(proc_data->prev_pipe);
	}
	if (current->next != NULL)
		dup2(proc_data->fd[1], STDOUT_FILENO);
	close(proc_data->fd[0]);
	close(proc_data->fd[1]);
}

void	handle_child_process(t_process_data *proc_data, t_node *current,
		t_exec_context *ctx)
{
	handle_child_io(proc_data, current);
	execute_single_command(current, ctx->env_list, ctx->exit_status, ctx->gc);
	my_exit(*(ctx->exit_status), ctx->gc);
}

void	handle_parent_io(t_process_data *proc_data, t_node *current, pid_t pid)
{
	if (proc_data->prev_pipe != STDIN_FILENO)
		close(proc_data->prev_pipe);
	close(proc_data->fd[1]);
	if (current->next == NULL)
	{
		close(proc_data->fd[0]);
		proc_data->last_pid = pid;
	}
	else
		proc_data->prev_pipe = proc_data->fd[0];
}

void	handle_pipe_and_multiple_commands(t_node *head, t_exec_context *ctx)
{
	t_node			*current;
	t_process_data	proc_data;

	current = head;
	proc_data.prev_pipe = dup(STDIN_FILENO);
	proc_data.last_pid = -1;
	while (current != NULL)
	{
		setup_pipe(&proc_data);
		proc_data.pid = fork();
		if (proc_data.pid == 0)
			handle_child_process(&proc_data, current, ctx);
		else if (proc_data.pid > 0)
		{
			handle_parent_io(&proc_data, current, proc_data.pid);
			current = current->next;
		}
		else
			return ;
	}
	wait_for_children(proc_data.last_pid, ctx->exit_status);
}
