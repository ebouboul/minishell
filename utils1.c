/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:34:20 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 03:39:26 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(char *input, int index)
{
	while (is_space(input[index]))
		index++;
	return (index);
}
int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == ';');
}
int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_quote2(char c)
{
	return (c == '"' || c == '\'');
}