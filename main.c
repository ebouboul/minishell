/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:17:11 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/22 22:00:14 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_and_execute_input(char *input, t_node **node, t_env *env_list,
		t_MemoryManager *manager)
{
	t_TokenInfo	*tokens;

	if (validate_input(input, &(*node)->exit_status))
	{
		tokens = process_input(input, &(*node)->exit_status, manager);
		free(input);
		if (!tokens)
			return ;
		*node = prepare_execution(tokens, env_list, (*node)->exit_status,
				manager);
		execute_cmds(*node, &env_list, &(*node)->exit_status, manager);
	}
	return ;
}

void	whiling(t_node *node, t_env *env_list, t_MemoryManager *manager)
{
	char	*input;

	input = NULL;
	while (1)
	{
		sigoo(node);
		input = readline("minishell$ ");
		sigoo(node);
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (!input || is_space1(input) == 1)
			continue ;
		add_history(input);
		process_and_execute_input(input, &node, env_list, manager);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_MemoryManager	*manager;
	t_env			*env_list;
	t_node			*node;

	(void)argc;
	(void)argv;
	manager = (t_MemoryManager *)malloc(sizeof(t_MemoryManager));
	manager->head = NULL;
	env_list = (t_env *)gc_malloc(manager, sizeof(t_env));
	node = (t_node *)gc_malloc(manager, sizeof(t_node));
	node->exit_status = 0;
	fill_env_list(manager, env, env_list);
	increment_shlvl(env_list, manager);
	whiling(node, env_list, manager);
	gc_free_all(manager);
	free(manager);
	rl_clear_history();
	return (0);
}
