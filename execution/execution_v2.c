/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:25 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/18 03:45:51 by ebouboul         ###   ########.fr       */
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

void print_error11(char *command, char *error)
{
	write(2, "minishell: ", 11);
	write(2, command, strlen(command));
	write(2, ": ", 2);
	write(2, error, strlen(error));
	write(2, "\n", 1);
}

int	check_file_permissions(char *file)
{
	struct stat	file_stat;
	int 		result;
	
	result = 0;
	if (stat(file, &file_stat) != 0)
	{
		print_error11(file, "No such file or directory");
		result = 127;
	}
	else if (S_ISDIR(file_stat.st_mode))
	{
		print_error11(file, "Is a directory");
		result = 126;
	}
	else if (access(file, X_OK) != 0)
	{
		print_error11(file, "Permission denied");
		result = 126;
	}
		exit(result);
	return (0);
}
