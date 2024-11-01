/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:10:44 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/26 20:41:57 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_node(t_env *node, t_MemoryManager *manager)
{
	if (node)
	{
		if (node->env)
		{
			gc_free(manager, node->env->key);
			gc_free(manager, node->env->value);
			gc_free(manager, node->env);
		}
		gc_free(manager, node);
	}
}

void	remove_env_node(t_env **env_list, t_env *node_to_remove, t_env *prev,
		t_MemoryManager *manager)
{
	if (prev == NULL)
		*env_list = node_to_remove->next;
	else
		prev->next = node_to_remove->next;
	free_env_node(node_to_remove, manager);
}

void	unset_variable(t_env **env_list, char *key, t_MemoryManager *manager)
{
	t_env	*current_env;
	t_env	*prev;

	current_env = *env_list;
	prev = NULL;
	while (current_env != NULL)
	{
		if (ft_strcmp(current_env->env->key, key) == 0)
		{
			remove_env_node(env_list, current_env, prev, manager);
			break ;
		}
		prev = current_env;
		current_env = current_env->next;
	}
}

int	ft_unset(t_command *command, t_env **env_list, t_MemoryManager *manager)
{
	int	i;

	if (!command || !env_list || !*env_list)
		return (1);
	if (command->args[1] == NULL)
	{
		print_error11("unset", "too few arguments");
		return (1);
	}
	i = 1;
	while (command->args[i] != NULL)
	{
		unset_variable(env_list, command->args[i], manager);
		i++;
	}
	return (0);
}
