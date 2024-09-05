/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 06:57:26 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/05 07:23:30 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_key_value_for_plus(char *var)
{
	char	**key_value;

	key_value = (char **)gc_malloc(3 * sizeof(char *));
	if (key_value == NULL)
	{
		perror("Memory allocation failed\n");
		exit(1);
	}
	key_value[0] = ft_strndup(var, ft_strchr(var, '+') - var);
	key_value[1] = ft_strdup(ft_strchr(var, '+') + 2);
	key_value[2] = NULL;
	return (key_value);
}

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

void	print_export_sorted(t_env *env_list)
{
	t_env	*current;
	char	**keys;
	char	**values;
	int		i;

	current = env_list;
	keys = (char **)gc_malloc(1000 * sizeof(char *));
	values = (char **)gc_malloc(1000 * sizeof(char *));
	i = 0;
	while (current != NULL)
	{
		keys[i] = ft_strdup(current->env->key);
		values[i] = ft_strdup(current->env->value);
		i++;
		current = current->next;
	}
	keys[i] = NULL;
	values[i] = NULL;
	ft_sort(keys, values);
	i = 0;
	while (keys[i] != NULL)
	{
		if (values[i] == NULL)
			printf("declare -x %s\n", keys[i]);
		else if (ft_strcmp(keys[i], "_") != 0)
			printf("declare -x %s=\"%s\"\n", keys[i], values[i]);
		i++;
	}
	gc_free(keys);
	gc_free(values);
}

int	is_plus_equal_case(char *arg)
{
	char	*plus;

	plus = ft_strchr(arg, '+');
	if (plus && plus[1] == '=')
		return (1);
	return (0);
}
int	check_export(char *args)
{
	int	i;

	i = 0;
	if (ft_isalpha(args[0]) == 0)
	{
		printf("export: not a valid identifier\n");
		return (1);
	}
	while (args[i] != '\0')
	{
		if (args[i] == '=' && (isalpha(args[i - 1]) == 0 && args[i - 1] != '_'
				&& args[i - 1] != '+'))
		{
			printf("export: not a valid identifier\n");
			return (1);
		}
		i++;
	}
	return (0);
}


void	add_new_env_entry(char *key, char *value, t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	while (current_env->next != NULL)
	{
		current_env = current_env->next;
	}
	current_env->next = (t_env *)gc_malloc(sizeof(t_env));
	current_env->next->env = (env *)gc_malloc(sizeof(env));
	current_env->next->env->key = ft_strdup(key);
	current_env->next->env->value = ft_strdup(value);
	current_env->next->next = NULL;
}

void	handle_export_key_value(char *key, char *value, t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	if (check_key_from_env(*env_list, key))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key) == 0)
			{
				gc_free(current_env->env->value);
				current_env->env->value = ft_strdup(value);
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
	{
		add_new_env_entry(key, value, env_list);
	}
}

void	handle_plus_equal_case(char *arg, t_env **env_list)
{
	t_env	*current_env;
	char	**key_value_plus;
	char	*new_value;

	current_env = *env_list;
	key_value_plus = get_key_value_for_plus(arg);
	if (check_key_from_env(*env_list, key_value_plus[0]))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key_value_plus[0]) == 0)
			{
				new_value = ft_strjoin(current_env->env->value,
						key_value_plus[1]);
				gc_free(current_env->env->value);
				current_env->env->value = new_value;
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
		add_new_env_entry(key_value_plus[0], key_value_plus[1], env_list);
	gc_free(key_value_plus);
}

void	handle_export_key_only(char *key, t_env **env_list)
{
	t_env	*current_env;

	current_env = *env_list;
	if (check_key_from_env(*env_list, key))
	{
		while (current_env != NULL)
		{
			if (ft_strcmp(current_env->env->key, key) == 0)
			{
				gc_free(current_env->env->value);
				current_env->env->value = ft_strdup("");
				break ;
			}
			current_env = current_env->next;
		}
	}
	else
		add_new_env_entry(key, NULL, env_list);
}

void	handle_export_arg(char *arg, t_env **env_list)
{
	char	**key_value;

	if (!check_export(arg))
	{
		key_value = get_key_value(arg);
		if (key_value[1] == NULL)
		{
			handle_export_key_only(key_value[0], env_list);
		}
		else if (is_plus_equal_case(arg))
		{
			handle_plus_equal_case(arg, env_list);
		}
		else
		{
			handle_export_key_value(key_value[0], key_value[1], env_list);
		}
		gc_free(key_value);
	}
}

int	ft_export(t_command *command, t_env **env_list)
{
	int	i;

	if (command->args[1] == NULL)
	{
		print_export_sorted(*env_list);
		return (0);
	}
	i = 1;
	while (command->args[i] != NULL)
	{
		handle_export_arg(command->args[i], env_list);
		i++;
	}
	return (0);
}
