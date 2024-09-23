/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:02:51 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/23 04:34:26 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_builtin_command(t_node *node, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	int	stdout_copy;
	int	stdin_copy;

	stdout_copy = dup(STDOUT_FILENO);
	stdin_copy = dup(STDIN_FILENO);
	*exit_status = handle_redirections(node, env_list, exit_status);
	if (*exit_status == 1)
		return ;
	*exit_status = execute_builtin(node, env_list, gc);
	dup2(stdout_copy, STDOUT_FILENO);
	dup2(stdin_copy, STDIN_FILENO);
	close(stdout_copy);
	close(stdin_copy);
}

void	handle_child_process1(t_node *node, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	char	*executable_path;
	char	**args;

	args = node->command->args;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(node, env_list, exit_status) == 1)
		my_exit(*exit_status, gc);
	if (args[0])
	{
		executable_path = find_executable_in_path(args[0], *env_list, gc);
		if (executable_path == NULL)
			*exit_status = check_file_permissions(args[0]);
		if (execve(executable_path, args,
				create_env_array(*env_list, gc)) == -1)
		{
			perror(args[0]);
			*exit_status = 127;
			my_exit(*exit_status, gc);
		}
	}
}

// void	execute_external_command(t_node *node, t_env **env_list,
// 		int *exit_status, t_MemoryManager *gc)
// {
// 	// pid_t	pid;

// 	// pid = fork();
// 	// if (pid == -1)
// 	// {
// 	// 	perror("fork");
// 	// 	exit(EXIT_FAILURE);
// 	// }
// 	// else if (pid == 0)
// 	// {
// 		handle_child_process1(node, env_list, exit_status, gc);
// 		my_exit(*exit_status, gc);
// 	// }
// 	// else
// 	// 	ft_waitpid(pid, exit_status);
// }

void	execute_single_command(t_node *node, t_env **env_list, int *exit_status,
		t_MemoryManager *gc)
{
	char	*cmd;

	if (node == NULL)
		return ;
	cmd = node->command->args[0];
	if (cmd && is_builtin(cmd))
		handle_builtin_command(node, env_list, exit_status, gc);
	else
	{
		handle_child_process1(node, env_list, exit_status, gc);
		my_exit(*exit_status, gc);
	}
}

char	*find_executable(const char *command, char **paths, t_MemoryManager *gc)
{
	int		i;
	char	*full_path;

	i = 0;
	if (ft_strlen(command) > 0)
	{
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
	}
	print_error11((char *)command, "command not found");
	my_exit(127, gc);
	return (NULL);
}
