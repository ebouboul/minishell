/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:49:05 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/08 17:31:34 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env *node)
{
	if (node)
	{
		if (node->env)
		{
			gc_free(node->env->key);
			gc_free(node->env->value);
			gc_free(node->env);
		}
		gc_free(node);
	}
}

void	remove_env_node(t_env **env_list, t_env *node_to_remove, t_env *prev)
{
	if (prev == NULL)
		*env_list = node_to_remove->next;
	else
		prev->next = node_to_remove->next;
	free_env_node(node_to_remove);
}

void	unset_variable(t_env **env_list, char *key)
{
	t_env	*current_env;
	t_env	*prev;

	current_env = *env_list;
	prev = NULL;
	while (current_env != NULL)
	{
		if (ft_strcmp(current_env->env->key, key) == 0
			&& ft_strcmp(current_env->env->key, "_") != 0)
		{
			remove_env_node(env_list, current_env, prev);
			break ;
		}
		prev = current_env;
		current_env = current_env->next;
	}
}

int	ft_unset(t_command *command, t_env **env_list)
{
	int	i;

	if (!command || !env_list || !*env_list)
		return (1);
	if (command->args[1] == NULL)
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	i = 1;
	while (command->args[i] != NULL)
	{
		unset_variable(env_list, command->args[i]);
		i++;
	}
	return (0);
}
