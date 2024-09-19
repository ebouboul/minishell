/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:52:41 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 03:13:56 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_key_value_for_plus(MemoryManager *manager, char *var)
{
	char	**key_value;

	key_value = (char **)gc_malloc(manager, 3 * sizeof(char *));
	key_value[0] = ft_strndup(manager, var, ft_strchr(var, '+') - var);
	key_value[1] = ft_strdup(manager, ft_strchr(var, '+') + 2);
	key_value[2] = NULL;
	return (key_value);
}

int	is_plus_equal_case(char *arg)
{
	char	*plus;

	plus = ft_strchr(arg, '+');
	if (plus && plus[1] == '=')
		return (1);
	return (0);
}

int	is_special_redc(char c)
{
	return (c == '<' || c == '>');
}

char	*ft_strjoin1(char c, char *s, MemoryManager *manager)
{
	char	*result;

	result = (char *)gc_malloc(manager, strlen(s) + 2);
	result[0] = c;
	strcpy(result + 1, s);
	return (result);
}

int	print_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (2);
}
