/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 20:33:19 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 02:20:48 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_input(char *input, int *exit_status)
{
	if (ft_strlen(input) == 0)
		return (0);
	if (track_quots(input) == 2)
	{
		(*exit_status) = 2;
		return (0);
	}
	return (1);
}

int	checking(t_TokenNode *list_head)
{
	if (check_special_chars(list_head) == 2)
		return (2);
	else if (check_syntax_double_special_cha(list_head) == 2)
		return (2);
	else if (check_syntax_special_face_to_face(list_head) == 2)
		return (2);
	else if (check_special_validity(list_head) == 2)
		return (2);
	else if (count_heredoc(list_head) == 2)
	{
		write(2, "minishell: too many heredocuments\n", 34);
		exit(2);
	}
	return (0);
}

t_TokenInfo	*process_input(char *input, int *exit_status,
		t_MemoryManager *manager)
{
	char		**inp;
	t_TokenInfo	*tokens;
	t_TokenNode	*list_head;

	input = add_spaces(input, manager);
	inp = ft_split(input, manager);
	tokens = tokenizer(inp, manager);
	if (!tokens)
		return (NULL);
	list_head = arrayintonodes(tokens, manager);
	if (checking(list_head) == 2)
	{
		*exit_status = 2;
		return (NULL);
	}
	return (tokens);
}

t_node	*prepare_execution(t_TokenInfo *tokens,
	t_env *env_list, int exit_status, t_MemoryManager *manager)
{
	t_TokenNode	*list_head;
	t_node		*node;

	list_head = arrayintonodes(tokens, manager);
	node = convert_to_node_list(list_head, manager);
	expansion_process(&node, env_list, exit_status, manager);
	replace_wildcard_in_args(node, manager);
	remove_quotes_and_join(node, manager);
	return (node);
}
