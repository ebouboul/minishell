/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:48:18 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 19:48:34 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_charjoin(char *s, char c, MemoryManager *gc)
{
	char	*result;
	int		i;
	int		len;

	len = ft_strlen(s);
	result = (char *)gc_malloc(gc, len + 2);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		result[i] = s[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

char	*get_env_value3(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, key) == 0)
			return (current->env->value);
		current = current->next;
	}
	return ("");
}

int	ft_strlen1(char **str)
{
	int	i;

	i = 0;
	while (str && str[i] != NULL)
	{
		i++;
	}
	return (i);
}

int	dollar_position(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

void	append_char(char c, char **new_arg, MemoryManager *gc)
{
	char temp[2] = {c, '\0'};
	char *old_arg;

	old_arg = *new_arg;
	*new_arg = ft_strjoin(old_arg, temp, gc);
	gc_free(gc, old_arg);
}