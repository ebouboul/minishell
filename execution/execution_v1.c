/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>                +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:18 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/19 22:43:58 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**execute_heredoc(t_node *current, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	char	**files;

	files = NULL;
	files = handle_heredoc(current, env_list, exit_status, gc);
	return (files);
}

void	exeall(t_node *current, t_exec_context *context)
{
	char	**files;

	files = NULL;
	if (is_heredoc(current))
		files = execute_heredoc(current, context->env_list,
				context->exit_status, context->gc);
	handle_pipe_and_multiple_commands(current, context, files);
	my_exit(*(context->exit_status), context->gc);
}

void	execute_cmds(t_node *head, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	t_node			*current;
	t_exec_context	context;
	pid_t			pid;

	current = head;
	context.env_list = env_list;
	context.exit_status = exit_status;
	context.gc = gc;
	context.file = NULL;
	sig_ignore();
	if (is_exe_bult(current, env_list, exit_status, gc))
		return ;
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			my_exit(1, gc);
		}
		else if (pid == 0)
			exeall(current, &context);
		else
			ft_waitpid(pid, exit_status);
	}
}

int	is_redirection(t_node *node)
{
	if (node == NULL || node->command == NULL || node->command->args == NULL
		|| node->command->args[0] == NULL)
		return (0);
	return (ft_strcmp(node->command->args[0], ">") == 0
		|| ft_strcmp(node->command->args[0], ">>") == 0
		|| ft_strcmp(node->command->args[0], "<") == 0);
}

int	count_env_variables(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}
