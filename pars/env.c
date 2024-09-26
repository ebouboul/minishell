/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:35:49 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/26 20:30:36 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(t_MemoryManager *manager, char *s, int n)
{
	char	*result;

	result = (char *)gc_malloc(manager, n + 1);
	strncpy(result, s, n);
	result[n] = '\0';
	return (result);
}

char	**set_env(t_MemoryManager *manager)
{
	char	*pwd;
	char	**envv;

	pwd = getcwd(NULL, 0);
	envv = (char **)gc_malloc(manager, 5 * sizeof(char *));
	envv[0] = ft_strjoin("PWD=", pwd, manager);
	envv[1] = ft_strdup(manager, "SHLVL=1");
	envv[2] = ft_strdup(manager, "_=/usr/bin/env");
	envv[3] = ft_strdup(manager, "OLDPWD");
	envv[4] = NULL;
	free(pwd);
	return (envv);
}

void	fill_env_list(t_MemoryManager *manager, char **envv,
	t_env **env_list)
{
	t_env	*current;
	int		i;
	char	**key_value;

	current = *env_list;
	i = 0;
	if (!envv[0])
		envv = set_env(manager);
	while (envv[i])
	{
		key_value = get_key_value(manager, envv[i]);
		current->env = (t_envv *)gc_malloc(manager, sizeof(t_envv));
		current->env->key = ft_strdup(manager, key_value[0]);
		current->env->value = ft_strdup(manager, key_value[1]);
		if (envv[i + 1])
		{
			current->next = (t_env *)gc_malloc(manager, sizeof(t_env));
			current = current->next;
		}
		else
			current->next = NULL;
		i++;
	}
}

char	**get_key_value(t_MemoryManager *manager, char *var)
{
	char	**key_value;

	key_value = (char **)gc_malloc(manager, 3 * sizeof(char *));
	if (ft_strchr(var, '=') == NULL)
	{
		key_value[0] = ft_strdup(manager, var);
		key_value[1] = ft_strdup(manager, "\20");
		key_value[2] = NULL;
		return (key_value);
	}
	key_value[0] = ft_strndup(manager, var, ft_strchr(var, '=') - var);
	key_value[1] = ft_strdup(manager, ft_strchr(var, '=') + 1);
	key_value[2] = NULL;
	return (key_value);
}

void	print_env_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (current->env->value != NULL && (current->env->value[0] != '\0')
			&& (current->env->value[0] != 16))
			printf("%s=%s \n", current->env->key, current->env->value);
		current = current->next;
	}
}
