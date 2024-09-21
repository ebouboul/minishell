/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:15:51 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 02:13:40 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	backtrack_wildcard(t_wildcard_vars *vars)
{
	if (vars->backtrackpindex != -1)
	{
		vars->pindex = vars->backtrackpindex + 1;
		vars->sindex = ++(vars->backtracksindex);
		return (1);
	}
	return (0);
}

int	if_qoilwild(char *pattern, char *str, t_wildcard_vars *vars)
{
	if (pattern[vars->pindex] == '*')
	{
		vars->backtrackpindex = vars->pindex;
		vars->backtracksindex = vars->sindex;
		vars->pindex++;
	}
	else if (pattern[vars->pindex] == str[vars->sindex])
	{
		vars->pindex++;
		vars->sindex++;
	}
	else if (!backtrack_wildcard(vars))
		return (0);
	return (1);
}

int	wildcard_match(char *pattern, char *str)
{
	t_wildcard_vars	vars;

	vars.pindex = 0;
	vars.sindex = 0;
	vars.backtrackpindex = -1;
	vars.backtracksindex = -1;
	if (str[0] == '.')
		return (0);
	while (str[vars.sindex] != '\0')
	{
		if (!if_qoilwild(pattern, str, &vars))
			return (0);
	}
	while (pattern[vars.pindex] == '*')
		vars.pindex++;
	return (pattern[vars.pindex] == '\0');
}
