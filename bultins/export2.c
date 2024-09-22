/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ansoulai <ansoulai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:49:19 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/22 21:15:44 by ansoulai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_export_key_value(t_MemoryManager *manager, char *key,
	char *value, t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	if (check_key_from_env(*env_list, key))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key) == 0)
			{
				gc_free(manager, current_env->env->value);
				current_env->env->value = ft_strdup(manager, value);
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
	{
		add_new_env_entry(manager, key, value, env_list);
	}
}

void	handle_plus_equal_case(t_MemoryManager *manager, char *arg,
		t_env **env_list)
{
	t_env	*current_env;
	char	**key_value_plus;
	char	*new_value;

	current_env = *env_list;
	key_value_plus = get_key_value_for_plus(manager, arg);
	if (check_key_from_env(*env_list, key_value_plus[0]))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key_value_plus[0]) == 0)
			{
				new_value = ft_strjoin(current_env->env->value,
						key_value_plus[1], manager);
				gc_free(manager, current_env->env->value);
				current_env->env->value = new_value;
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
		add_new_env_entry(manager, key_value_plus[0], key_value_plus[1],
			env_list);
	gc_free(manager, key_value_plus);
}

void	handle_export_key_only(t_MemoryManager *manager, char *key,
		t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	if (check_key_from_env(*env_list, key))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key) == 0)
			{
				gc_free(manager, current_env->env->value);
				current_env->env->value = ft_strdup(manager, "");
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
		add_new_env_entry(manager, key, NULL, env_list);
}

int	check_export(char *args)
{
	int	i;

	i = 0;
	if (args[0] == '=' )
	{
		print_error11(args, "not a valid identifier");
		return (1);
	}
	while (args[i] != '\0' && args[i] != '=')
	{
		if (ft_isalpha(args[i]) == 0)
		{
			print_error11(args, "not a valid identifier");
			return (1);
		}
		else if (args[i] == '='
			&& (isalnum(args[i - 1]) == 0 && args[i - 1] != '_'
				&& args[i - 1] != '+'))
		{
			print_error11(args, "not a valid identifier");
			return (1);
		}
		i++;
	}
	return (0);
}

int	handle_export_arg(t_MemoryManager *manager, char *arg, t_env **env_list)
{
	char	**key_value;

	if (!check_export(arg))
	{
		key_value = get_key_value(manager, arg);
		if (key_value[1] == NULL)
			handle_export_key_only(manager, key_value[0], env_list);
		else if (is_plus_equal_case(arg))
			handle_plus_equal_case(manager, arg, env_list);
		else
			handle_export_key_value(manager, key_value[0], key_value[1],
				env_list);
		gc_free(manager, key_value);
		return (0);
	}
	return (1);
}
