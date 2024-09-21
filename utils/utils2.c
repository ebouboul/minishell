/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:05:05 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:55:20 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ret_check(t_TokenNode *current)
{
	return ((current->next != NULL && current->next->info.type == TOKEN_APPEND)
		|| (current->next != NULL && current->next->info.type == TOKEN_HEREDOC)
		|| (current->next != NULL
			&& current->next->info.type == TOKEN_REDIRECT_IN)
		|| (current->next != NULL
			&& current->next->info.type == TOKEN_REDIRECT_OUT));
}

void	my_exit(int status, t_MemoryManager *manager)
{
	if (manager)
		gc_free_all(manager);
	free(manager);
	rl_clear_history();
	exit(status);
}

int	is_special_char2(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<'
		|| c == '>')
		return (1);
	return (0);
}

int	is_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*' && !ft_strchr(str, '\'') && !ft_strchr(str, '\"'))
			return (1);
		i++;
	}
	return (0);
}

int	is_wildcard_in_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		if (is_wildcard(args[i]))
			return (1);
		i++;
	}
	return (0);
}
