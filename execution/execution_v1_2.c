/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_v1_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peaky <peaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:03:37 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/19 22:45:37 by peaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// norm=OK!
char	*create_env_string(t_env *env, MemoryManager *gc)
{
	char	*env_string;

	env_string = gc_malloc(gc, strlen(env->env->key)
			+ ft_strlen(env->env->value) + 2);
	env_string = ft_strdup(gc, env->env->key);
	env_string = ft_strjoin(env_string, "=", gc);
	env_string = ft_strjoin(env_string, env->env->value, gc);
	return (env_string);
}

char	**create_env_array(t_env *env_list, MemoryManager *gc)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;

	count = count_env_variables(env_list);
	env_array = gc_malloc(gc, sizeof(char *) * (count + 1));
	current = env_list;
	i = 0;
	while (current != NULL)
	{
		env_array[i] = create_env_string(current, gc);
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

char	**split_path(const char *path, MemoryManager *gc)
{
	char	*path_copy;
	int		count;
	char	**paths;
	int		i;
	char	**token;

	path_copy = strdup(path);
	count = count_path_components(path);
	paths = gc_malloc(gc, sizeof(char *) * (count + 1));
	i = 0;
	token = ft_split3(path_copy, ':', gc);
	while (token[i] != NULL)
	{
		paths[i] = ft_strdup(gc, token[i]);
		i++;
	}
	paths[i] = NULL;
	free(path_copy);
	return (paths);
}
