/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:43:57 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 06:36:24 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exp_reddd(char **args, t_env *env_list, int exit_status,
		t_MemoryManager *gc)
{
	t_exec_context	context;
	char			*last;

	int (i), (j), (k);
	context.env_list = &env_list;
	context.exit_status = &exit_status;
	context.gc = gc;
	i = 0;
	while (args[i] != NULL)
	{
		last = args[i];
		k = 0;
		while (last && dstrchr(last, '$', &j) && !is_dollar_only(last))
		{
			handle_expansion(args, i, &context, &k);
			change_qoutes(args[i]);
			if (k >= (int)ft_strlen(args[i]))
				break ;
			last = args[i] + k;
		}
		i++;
	}
}

void	expand_redirect(t_redirect **redirect, t_env *env_list, int exit_status,
		t_MemoryManager *gc)
{
	t_redirect	*current;
	char		**args;

	current = *redirect;
	while (current)
	{
		if (current->flag != 8)
		{
			args = (char **)gc_malloc(gc, sizeof(char *) * 2);
			args[0] = current->str;
			args[1] = NULL;
			exp_reddd(args, env_list, exit_status, gc);
			current->str = args[0];
		}
		current = current->next;
	}
}

int	need_expansion(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	expan_herdoc(char **args, t_env *env_list, int exit_status,
		t_MemoryManager *gc)
{
	char			*last;
	t_exec_context	context;

	int (i), (k);
	context.env_list = &env_list;
	context.exit_status = &exit_status;
	context.gc = gc;
	i = 0;
	while (args[i] != NULL)
	{
		last = args[i];
		k = 0;
		while (last && need_expansion(last) && !is_dollar_only(last))
		{
			handle_expansion(args, i, &context, &k);
			// change_qoutes(args[i]);
			if (k >= (int)ft_strlen(args[i]))
				break ;
			last = args[i] + k;
		}
		i++;
	}
}

void	expansion_process(t_node **head, t_env *env_list, int exit_status,
		t_MemoryManager *gc)
{
	t_node			*current;
	t_command		*current_command;
	t_exec_context	context;

	current = *head;
	context.env_list = &env_list;
	context.exit_status = &exit_status;
	context.gc = gc;
	while (current != NULL)
	{
		current_command = current->command;
		while (current_command != NULL)
		{
			process_arguments(current_command, &context);
			if (current_command->redirect)
				expand_redirect(&current_command->redirect, env_list,
					exit_status, gc);
			current_command = current_command->next;
		}
		current = current->next;
	}
}
