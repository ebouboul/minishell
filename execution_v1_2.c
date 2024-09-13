/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:03:37 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/13 17:49:15 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// norm=OK! //strtok + **functions
char	*create_env_string(t_env *env)
{
	char	*env_string;

	env_string = malloc(strlen(env->env->key) + ft_strlen(env->env->value) + 2);
	sprintf(env_string, "%s=%s", env->env->key, env->env->value);
	return (env_string);
}

char	**create_env_array(t_env *env_list)
{
	int		count;
	char	**env_array = malloc(sizeof(char *) * (count + 1));
	t_env	*current;
	int		i;

	count = count_env_variables(env_list);
	current = env_list;
	i = 0;
	while (current != NULL)
	{
		env_array[i] = create_env_string(current);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	count_path_components(const char *path)
{
	int			count;
	const char	*ptr;

	count = 0;
	ptr = path;
	while (*ptr != '\0')
	{
		if (*ptr == ':')
			count++;
		ptr++;
	}
	return (count + 1);
}

char	**split_path(const char *path)
{
	char	*path_copy;
	int		count;
	char	**paths = malloc(sizeof(char *) * (count + 1));
	int		i;
	char	*token;

	token = strtok(path_copy, ":");
	path_copy = strdup(path);
	count = count_path_components(path);
	i = 0;
	while (token != NULL)
	{
		paths[i++] = strdup(token);
		token = strtok(NULL, ":");
	}
	paths[i] = NULL;
	free(path_copy);
	return (paths);
}
