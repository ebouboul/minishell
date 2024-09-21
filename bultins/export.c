/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 06:57:26 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 05:57:29 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sort(char **keys, char **values)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	while (keys[i] != NULL)
	{
		j = i + 1;
		while (keys[j] != NULL)
		{
			if (ft_strcmp(keys[i], keys[j]) > 0)
			{
				temp = keys[i];
				keys[i] = keys[j];
				keys[j] = temp;
				temp = values[i];
				values[i] = values[j];
				values[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_export_values(char **keys, char **values)
{
	int	i;

	i = 0;
	while (keys[i] != NULL)
	{
		if (values[i] == NULL || values[i][0] == 16)
			printf("declare -x %s\n", keys[i]);
		else if (ft_strcmp(keys[i], "_") != 0)
			printf("declare -x %s=\"%s\"\n", keys[i], values[i]);
		i++;
	}
}

void	print_export_sorted(t_MemoryManager *manager, t_env *env_list)
{
	t_env	*current;
	char	**keys;
	char	**values;
	int		i;

	current = env_list;
	keys = (char **)gc_malloc(manager, 1000 * sizeof(char *));
	values = (char **)gc_malloc(manager, 1000 * sizeof(char *));
	i = 0;
	while (current != NULL)
	{
		keys[i] = ft_strdup(manager, current->env->key);
		values[i] = ft_strdup(manager, current->env->value);
		i++;
		current = current->next;
	}
	keys[i] = NULL;
	values[i] = NULL;
	ft_sort(keys, values);
	print_export_values(keys, values);
	gc_free(manager, keys);
	gc_free(manager, values);
}

void	add_new_env_entry(t_MemoryManager *manager, char *key, char *value,
		t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	while (current_env->next != NULL)
	{
		current_env = current_env->next;
	}
	current_env->next = (t_env *)gc_malloc(manager, sizeof(t_env));
	current_env->next->env = (t_envv *)gc_malloc(manager, sizeof(t_envv));
	current_env->next->env->key = ft_strdup(manager, key);
	if (value != NULL)
		current_env->next->env->value = ft_strdup(manager, value);
	current_env->next->next = NULL;
}

int	ft_export(t_command *command, t_env **env_list, t_MemoryManager *manager)
{
	int	i;

	if (command->args[1] == NULL)
	{
		print_export_sorted(manager, *env_list);
		return (0);
	}
	i = 1;
	while (command->args[i] != NULL)
	{
		if (handle_export_arg(manager, command->args[i], env_list) == 1)
			return (1);
		i++;
	}
	return (0);
}
