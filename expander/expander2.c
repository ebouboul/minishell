/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:44:57 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 19:54:38 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_exit_status(char **new_arg, int *j, int exit_status,
		MemoryManager *gc)
{
	char	*value;

	*new_arg = ft_strdup(gc, "");
	value = ft_itoa(exit_status, gc);
	append_expanded_value(new_arg, value, gc);
	*j += 2;
}

void	handle_expansion(char **args, int i, t_env *env_list, int exit_status,
		int *k, MemoryManager *gc)
{
	char	*new_arg;

	int(*j),
		(flag)
			j = 0;
	flag = 0;
	new_arg = ft_strdup(gc, "");
	while (args[i][j] != '\0')
	{
		if (args[i][j] == '$' && args[i][j + 1] != '\0')
		{
			if (args[i][j + 1] == '?')
				handle_exit_status(&new_arg, &j, exit_status, gc);
			else if (flag == 0)
			{
				expand_variable(args[i], &new_arg, &j, env_list, k, gc);
				flag = 1;
			}
			else
				append_char(args[i][j++], &new_arg, gc);
			*k = j;
		}
		else
		{
			append_char(args[i][j++], &new_arg, gc);
			*k = j;
		}
	}
	update_args(args, new_arg, i);
}
