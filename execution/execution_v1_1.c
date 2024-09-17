/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:02:51 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/17 02:37:38 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// NORM=OK! //one func with more 25 lines
void	handle_builtin_command(t_node *node, t_env **env_list, int *exit_status, MemoryManager *gc)
{
	int	stdout_copy;
	int	stdin_copy;

	stdout_copy = dup(STDOUT_FILENO);
	stdin_copy = dup(STDIN_FILENO);
	handle_redirections(node, env_list, exit_status);
	*exit_status = execute_builtin(node, env_list, gc);
	dup2(stdout_copy, STDOUT_FILENO);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdout_copy);
	close(stdin_copy);
}


void	execute_external_command(t_node *node,
		t_env **env_list, int *exit_status, MemoryManager *gc)
{
	pid_t	pid;
	int		status;
	char	*executable_path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		handle_redirections(node, env_list, exit_status);
		if (node->command->args[0])
		{
			executable_path = find_executable_in_path
				(node->command->args[0], *env_list, gc);
			if (execve(executable_path,
					node->command->args, create_env_array(*env_list, gc)) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else
			*exit_status = 1;
	}
}

void	execute_single_command(t_node *node, t_env **env_list, int *exit_status, MemoryManager *gc)
{
	char	*cmd;

	if (node == NULL)
		return ;
	cmd = node->command->args[0];
	if (cmd && is_builtin(cmd))
		handle_builtin_command(node, env_list, exit_status, gc);
	else
		execute_external_command(node, env_list, exit_status, gc);
}

char	*find_executable(const char *command, char **paths, MemoryManager *gc)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i] != NULL)
	{
		full_path = gc_malloc(gc, strlen(paths[i]) + strlen(command) + 2);
		full_path = ft_strjoin(paths[i], "/", gc);
		full_path = ft_strjoin(full_path, command, gc);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		gc_free(gc, full_path);
		i++;
	}
	return (NULL);
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
