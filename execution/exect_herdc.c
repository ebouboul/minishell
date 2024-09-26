/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect_herdc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:43:54 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/26 21:56:19 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_child_process11(t_node *node, t_process_data *pdata,
		char *temp_file, t_exec_context *context)
{
	if (pdata->prev_pipe != -1)
	{
		dup2(pdata->prev_pipe, STDIN_FILENO);
		close(pdata->prev_pipe);
	}
	if (node->next != NULL)
	{
		close(pdata->fd[0]);
		dup2(pdata->fd[1], STDOUT_FILENO);
		close(pdata->fd[1]);
	}
	if (!is_herefoc1(node))
		close(2);
	execute_command_with_heredoc(node, temp_file, context);
	exit(EXIT_SUCCESS);
}

int	handle_parent_process(t_process_data *pdata, t_node *next_node)
{
	if (next_node != NULL)
		close(pdata->fd[1]);
	if (pdata->prev_pipe != -1)
		close(pdata->prev_pipe);
	if (next_node != NULL)
		return (pdata->fd[0]);
	else
		return (-1);
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

int	handle_piping_and_forking(t_node *node, t_process_data *pdata,
		char *temp_file, t_exec_context *context)
{
	if (node->next && pipe(pdata->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pdata->pid = fork();
	if (pdata->pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pdata->pid == 0)
		handle_child_process11(node, pdata, temp_file, context);
	else
		pdata->prev_pipe = handle_parent_process(pdata, node->next);
	return (pdata->prev_pipe);
}
