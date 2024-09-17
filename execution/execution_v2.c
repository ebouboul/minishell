/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:25 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/17 02:36:08 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// NORM=OK!
char	*gett_env_value(const char *key, t_env *env_list)
{
	while (env_list != NULL)
	{
		if (strcmp(env_list->env->key, key) == 0)
			return (env_list->env->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*find_executable_in_path(char *command, t_env *env_list, MemoryManager *gc)

{
	char	*path_value;
	char	*executable_path;
	int		i;
	char	**paths;

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(gc, command));
		else
			return (NULL);
	}
	path_value = gett_env_value("PATH", env_list);
	if (path_value == NULL)
		return (NULL);
	paths = split_path(path_value, gc);
	executable_path = find_executable(command, paths, gc);
	i = 0;
	while (paths && paths[i] != NULL)
	{
		gc_free(gc, paths[i]);
		i++;
	}
	gc_free(gc, paths);
	return (executable_path);
}

int	check_file_permissions(char *file)
{
	struct stat	file_stat;

	if (stat(file, &file_stat) != 0)
		return (-1);
	if (S_ISDIR(file_stat.st_mode))
	{
		fprintf(stderr, "minishell: %s: is a directory\n", file);
		return (126);
	}
	if (access(file, X_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", file);
		return (126);
	}
	return (0);
}

int	execute_command(char *executable_path, char **args, t_env *env_list, MemoryManager *gc)
{
	pid_t		pid;
	int			status;
	char		**env_array;

	pid = fork();
	if (pid == 0)
	{
		env_array = create_env_array(env_list, gc);
		execve(executable_path, args, env_array);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
}

int	execute_external(t_command *command, t_env *env_list, MemoryManager *gc)
{
	char	*executable_path;
	int		result;
	int		check_result;

	executable_path = find_executable_in_path(command->args[0], env_list, gc);
	if (executable_path == NULL)
	{
		check_result = check_file_permissions(command->args[0]);
		if (check_result == 0)
			executable_path = ft_strdup(gc, command->args[0]);
		else if (check_result > 0)
			return (check_result);
	}
	if (executable_path == NULL)
	{
		fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
		return (127);
	}
	result = execute_command(executable_path, command->args, env_list, gc);
	gc_free(gc, executable_path);
	return (result);
}
// {
// 	char	*executable_path;
// 	int		result;
// 	int		check_result;

// 	executable_path = find_executable_in_path(command->args[0], env_list);
// 	if (executable_path == NULL)
// 	{
// 		check_result = check_file_permissions(command->args[0]);
// 		if (check_result == 0)
// 			executable_path = strdup(command->args[0]);
// 		else if (check_result > 0)
// 			return (check_result);
// 	}
// 	if (executable_path == NULL)
// 	{
// 		fprintf(stderr, "minishell: %s: command not found\n", command->args[0]);
// 		return (127);
// 	}
// 	result = execute_command(executable_path, command->args, env_list);
// 	gc_free(executable_path);
// 	return (result);
// }
