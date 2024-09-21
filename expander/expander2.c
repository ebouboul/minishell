/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:44:57 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 06:34:40 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_exit_status(char **new_arg, int *j, int exit_status,
		t_MemoryManager *gc)
{
	char	*value;

	*new_arg = ft_strdup(gc, "");
	value = ft_itoa(exit_status, gc);
	append_expanded_value(new_arg, value, gc);
	*j += 2;
}

void	handle_special_case(char **new_arg, char *arg, int *j,
		t_exec_context *context)
{
	int		flag;

	flag = 0;
	if (arg[*j] == '$' && arg[*j + 1] == '?')
	{
		handle_exit_status(new_arg, j, *context->exit_status, context->gc);
		flag = 1;
	}
	else if (flag == 0)
	{
		expand_variable(arg, new_arg, j, context);
		flag = 1;
	}
}

void	handle_expansion(char **args, int i, t_exec_context *context, int *k)
{
	char	*new_arg;
	int		j;

	j = 0;
	context->k = k;
	new_arg = ft_strdup(context->gc, "");
	while (args[i][j] != '\0')
	{
		if (args[i][j] == '$' && args[i][j + 1] != '\0')
			handle_special_case(&new_arg, args[i], &j, context);
		else
			append_char(args[i][j++], &new_arg, context->gc);
		*k = j;
	}
	update_args(args, new_arg, i);
}

int	should_split_argument(char **args, int i, char *last)
{
	return (args[i] && ft_strchr(args[i], ' ') != NULL
		&& (ft_strncmp(args[0], "export", 6) != 0
			|| (ft_strncmp(args[0], "export", 6) == 0
				&& args[i][0] != '$'))
		&& is_last_dollar(args[i], '$') == 0
		&& double_quotes(last) == 0
		&& is_space1(args[i]) == 0);
}
