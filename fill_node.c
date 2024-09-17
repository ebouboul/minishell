/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:23:01 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 02:17:40 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*initialize_command(MemoryManager *manager)
{
	t_command	*cmd;

	cmd = gc_malloc(manager, sizeof(t_command));
	cmd->args = gc_malloc(manager, sizeof(char *) * MAX_TOKENS);
	cmd->redirect = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirection(MemoryManager *manager, t_command *cmd, TokenNode **current)
{
    t_redirect	*redir;
    t_redirect	*last_redir;

    if ((*current)->info.type == TOKEN_REDIRECT_IN
        || (*current)->info.type == TOKEN_REDIRECT_OUT
        || (*current)->info.type == TOKEN_APPEND
        || (*current)->info.type == TOKEN_HEREDOC)
    {
        redir = gc_malloc(manager, sizeof(t_redirect));
        redir->str = ft_strdup(manager, (*current)->next->info.value);
        redir->flag = (*current)->info.type;
        redir->next = NULL;        
        if (cmd->redirect == NULL)
            cmd->redirect = redir;
        else
        {
            last_redir = cmd->redirect;
            while (last_redir->next != NULL)
                last_redir = last_redir->next;
            last_redir->next = redir;
        }
        *current = (*current)->next;
    }
}

t_command	*convert_to_command(MemoryManager *manager, TokenNode **current)
{
	t_command	*cmd;
	int			arg_count;

	arg_count = 0;
	cmd = initialize_command(manager);
	while (*current && (*current)->info.type != TOKEN_PIPE)
	{
		if ((*current)->info.type == TOKEN_COMMAND
			|| (*current)->info.type == TOKEN_ARG)
			cmd->args[arg_count++] = ft_strdup(manager, (*current)->info.value);
		else if ((*current)->info.type == TOKEN_REDIRECT_IN
			|| (*current)->info.type == TOKEN_REDIRECT_OUT
			|| (*current)->info.type == TOKEN_APPEND
			|| (*current)->info.type == TOKEN_HEREDOC)
			handle_redirection(manager, cmd, current);
		*current = (*current)->next;
	}
	cmd->args[arg_count] = NULL;
	return (cmd);
}

t_node	*convert_to_node_list(TokenNode *token_list, MemoryManager *manager)
{
	t_node	*node_list;
	t_node	*last_node;
	t_node	*new_node;

	node_list = NULL;
	last_node = NULL;
	while (token_list)
	{
		new_node = gc_malloc(manager, sizeof(t_node));
		new_node->command = convert_to_command(manager, &token_list);
		new_node->exit_status = 0;
		new_node->next = NULL;
		if (!node_list)
		{
			node_list = new_node;
		}
		else
			last_node->next = new_node;
		last_node = new_node;
		if (token_list && token_list->info.type == TOKEN_PIPE)
			token_list = token_list->next;
	}
	return (node_list);
}

void	print_node_list(t_node *node_list)
{
	t_node		*current;
	int			i;
	t_redirect	*redir;

	current = node_list;
	while (current != NULL)
	{
		i = 0;
		while (current->command->args && current->command->args[i])
		{
			printf("Arg: %s\n", current->command->args[i]);
			i++;
		}
		redir = current->command->redirect;
		while (redir != NULL)
		{
			printf("Redirect: %s, Type: %d\n", redir->str, redir->flag);
			redir = redir->next;
		}
		if (current->next)
		{
			printf("next\n");
		}
		current = current->next;
	}
}
