/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:57:40 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:49:14 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_key_from_env(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, key) == 0)
		{
			return (1);
		}
		current = current->next;
	}
	return (0);
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, key) == 0)
			return (current->env->value);
		current = current->next;
	}
	return (NULL);
}

void	replace_env_value(t_env **env_list, char *key, char *value,
		t_MemoryManager *manager)
{
	t_env	*current;

	current = *env_list;
	if (value == NULL)
		return ;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, key) == 0)
		{
			current->env->value = ft_strdup(manager, value);
			break ;
		}
		current = current->next;
	}
}

int	ft_pwd(t_env *env_list, t_MemoryManager *gc)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
	{
		printf("PWD=%s\n", path);
		free(path);
	}
	else
	{
		path = ft_strdup(gc, get_env_value(env_list, "PWD"));
		if (path)
			printf("PWD=%s\n", path);
		gc_free(gc, path);
	}
	return (0);
}

int	ft_env(t_command *command, t_env *env_list)
{
	t_env	*current;

	current = env_list;
	if (command->args[1] == NULL)
	{
		print_env_list(env_list);
		return (0);
	}
	else
	{
		print_error11("env", "too many arguments");
		return (1);
	}
}
