/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:34:20 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/23 20:10:13 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_quote2(char c)
{
	return (c == '"' || c == '\'');
}

int	is_dollar_only(char *str)
{
	int i;
	i = 0;
	while(str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] != '\0')
			return (0);
		i++;
	}
	if ((str[ft_strlen(str) - 1] == '$'))
		return (1);
	return (0);
}

int	is_space1(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] > 32)
			return (0);
		i++;
	}
	return (1);
}

int	count_heredoc(t_TokenNode *list_head)
{
	int			count;
	t_TokenNode	*temp;

	count = 0;
	temp = list_head;
	while (temp)
	{
		if (temp->info.type == TOKEN_HEREDOC)
			count++;
		temp = temp->next;
	}
	if (count > 16)
		return (2);
	return (0);
}
