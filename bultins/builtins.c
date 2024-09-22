/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:11:18 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/22 21:17:44 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1 && s2 && s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	else if (ft_strcmp(command, "cd") == 0)
		return (1);
	else if (ft_strcmp(command, "pwd") == 0)
		return (1);
	else if (ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strcmp(command, "unset") == 0)
		return (1);
	else if (ft_strcmp(command, "env") == 0)
		return (1);
	else if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_node *head, t_env **env_list, t_MemoryManager *manager)
{
	t_node	*current;
	int		status;

	current = head;
	status = 0;
	if (ft_strcmp(current->command->args[0], "echo") == 0)
		status = ft_echo(current->command);
	else if (ft_strcmp(current->command->args[0], "cd") == 0)
		status = ft_cd(current->command, env_list, manager);
	else if (ft_strcmp(current->command->args[0], "pwd") == 0)
		status = ft_pwd(*env_list, manager);
	else if (ft_strcmp(current->command->args[0], "export") == 0)
		status = ft_export(current->command, env_list, manager);
	else if (ft_strcmp(current->command->args[0], "unset") == 0)
		status = ft_unset(current->command, env_list, manager);
	else if (ft_strcmp(current->command->args[0], "env") == 0)
		status = ft_env(current->command, *env_list);
	else if (ft_strcmp(current->command->args[0], "exit") == 0)
		status = ft_exit(current->command, manager);
	return (status);
}
