/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 03:34:20 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/20 03:36:10 by ebouboul         ###   ########.fr       */
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
	if ((str[0] == '$' && str[1] == '\0' ))
		return (1);
	return (0);
}
