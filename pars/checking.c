/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:24:30 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 02:21:28 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes_spiclal_chars(t_TokenNode *head, char c)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_COMMAND
			|| current->info.type == TOKEN_ARG)
		{
			if (current->info.value[0] == c && current->next != NULL
				&& current->next->info.type != TOKEN_COMMAND
				&& current->next->info.type != TOKEN_ARG)
				return (print_error(
						"Error: special char not allowed after quotes\n"));
		}
		current = current->next;
	}
	return (0);
}

int	check_syntax_double_commands(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_COMMAND)
		{
			if (current->next != NULL
				&& current->next->info.type == TOKEN_COMMAND)
				return (print_error("Error: Double commands\n"));
		}
		current = current->next;
	}
	return (0);
}

int	check_special_chars(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	if (current->info.type == TOKEN_PIPE)
		return (print_error(
				"Error: Special character at the beginning of the command\n"));
	while (current != NULL)
	{
		if (current->info.type == TOKEN_PIPE
			|| current->info.type == TOKEN_REDIRECT_IN
			|| current->info.type == TOKEN_REDIRECT_OUT
			|| current->info.type == TOKEN_APPEND
			|| current->info.type == TOKEN_HEREDOC)
		{
			if (current->next == NULL)
				return (print_error(
						"Error: Special char at the end of the command\n"));
		}
		current = current->next;
	}
	return (0);
}

int	check_syntax_double_special_cha(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_APPEND
			|| current->info.type == TOKEN_HEREDOC
			|| current->info.type == TOKEN_REDIRECT_IN
			|| current->info.type == TOKEN_REDIRECT_OUT)
		{
			if (current->next != NULL && current->next->info.type != TOKEN_FILE)
				return (print_error("Error: Double special characters\n"));
		}
		current = current->next;
	}
	return (0);
}

int	check_syntax_special_face_to_face(t_TokenNode *head)
{
	t_TokenNode	*current;

	current = head;
	while (current != NULL)
	{
		if (current->info.type == TOKEN_APPEND
			|| current->info.type == TOKEN_HEREDOC
			|| current->info.type == TOKEN_REDIRECT_IN
			|| current->info.type == TOKEN_REDIRECT_OUT)
		{
			if (ret_check(current))
				return (print_error("Error: Face to Face\n"));
		}
		else if (current->info.type == TOKEN_PIPE)
		{
			if (current->next == NULL || current->next->info.type == TOKEN_PIPE)
				return (print_error("Error: Face to Face\n"));
		}
		current = current->next;
	}
	return (0);
}
