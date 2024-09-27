/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exect.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 01:34:29 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/27 19:05:06 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_exe_bult(t_node *node, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	t_node	*current;
	char	*cmd;

	current = node;
	sig_ignore();
	cmd = current->command->args[0];
	if (cmd && is_builtin(cmd) && current->next == NULL)
	{
		handle_builtin_command(node, env_list, exit_status, gc);
		return (1);
	}
	return (0);
}

int	is_heredoc(t_node *node)
{
	t_redirect	*redirect;
	t_node		*current;

	if (!node)
		return (0);
	current = node;
	while (current)
	{
		if (current->command && current->command->redirect)
		{
			redirect = current->command->redirect;
			while (redirect)
			{
				if (redirect->flag == 8)
					return (1);
				redirect = redirect->next;
			}
		}
		current = current->next;
	}
	return (0);
}

int	is_herefoc1(t_node *node)
{
	t_redirect	*redirect;
	if (!node)
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
