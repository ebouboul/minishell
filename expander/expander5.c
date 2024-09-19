/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:46:48 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 19:46:53 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_var_name(char *arg, int *j, MemoryManager *gc)
{
	int		k;
	char	*var_name;

	k = *j;
	if (arg[k] == '$' && arg[k + 1] == '$')
	{
		*j = k + 2;
		return (ft_strdup(gc, "$"));
	}
	while (arg[k] && (isalpha(arg[k + 1]) || isdigit(arg[k + 1]) || arg[k
			+ 1] == '_'))
		k++;
	if (ft_strchr(ft_substr(arg, *j, k, gc), '\'') != NULL
			&& ft_strchr(ft_substr(arg, *j, k, gc), '"') == NULL
			&& ft_strchr(ft_substr(arg, *j, k + 1, gc), '\'')[1] != '"')
		return (NULL);
	var_name = ft_substr(arg, *j + 1, k - *j, gc);
	if (!var_name)
		return (NULL);
	*j = k + 1;
	return (var_name);
}

char	*get_variable_value(t_env *env_list, char *var_name, MemoryManager *gc)
{
	char	*value;

	if (isdigit(var_name[0]))
		value = ft_strdup(gc, var_name + 1);
	else
		value = get_env_value3(env_list, var_name);
	if (!value)
		return (NULL);
	return (value);
}

void	append_expanded_value(char **new_arg, char *value, MemoryManager *gc)
{
	char	*temp;

	temp = *new_arg;
	*new_arg = ft_strjoin(temp, value, gc);
	gc_free(gc, temp);
}

int	is_last_dollar(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	if (str[i - 1] == c)
		return (1);
	return (0);
}
void	expand_variable(char *arg, char **new_arg, int *j, t_env *env_list,
		int *k, MemoryManager *gc)
{
	char *var_name;
	char *value;

	if (arg == NULL || *j >= (int)ft_strlen(arg))
		return ;
	var_name = NULL;
	value = NULL;
	var_name = extract_var_name(arg, j, gc);
	if (var_name)
	{
		value = get_variable_value(env_list, var_name, gc);
		if (value)
			append_expanded_value(new_arg, value, gc);
		if (value[0] != '\0')
			*k = *j;
	}
	if (*j < (int)ft_strlen(arg))
		append_char(arg[(*j)++], new_arg, gc);
}