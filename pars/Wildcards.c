/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:15:51 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/20 02:38:31 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	backtrack_wildcard(t_wildcard_vars *vars)
{
	if (vars->backtrackPIndex != -1)
	{
		vars->pIndex = vars->backtrackPIndex + 1;
		vars->sIndex = ++(vars->backtrackSIndex);
		return (1);
	}
	return (0);
}

int	if_qoilwild(char *pattern, char *str, t_wildcard_vars *vars)
{
	if (pattern[vars->pIndex] == '*')
	{
		vars->backtrackPIndex = vars->pIndex;
		vars->backtrackSIndex = vars->sIndex;
		vars->pIndex++;
	}
	else if (pattern[vars->pIndex] == str[vars->sIndex])
	{
		vars->pIndex++;
		vars->sIndex++;
	}
	else if (!backtrack_wildcard(vars))
		return (0);
	return (1);
}

int	wildcard_match(char *pattern, char *str)
{
	t_wildcard_vars	vars;

	vars.pIndex = 0;
	vars.sIndex = 0;
	vars.backtrackPIndex = -1;
	vars.backtrackSIndex = -1;
	if (str[0] == '.')
		return (0);
	while (str[vars.sIndex] != '\0')
	{
		if (!if_qoilwild(pattern, str, &vars))
			return (0);
	}
	while (pattern[vars.pIndex] == '*')
		vars.pIndex++;
	return (pattern[vars.pIndex] == '\0');
}
