/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:44:37 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 06:36:34 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_qouted(char *str)
{
	if (str[ft_strlen(str) - 1] == '\"' || str[ft_strlen(str) - 1] == '\'')
		return (1);
	return (0);
}

void	change_qoutes1(char *str)
{
	int (i), (j);
	i = 0;
	j = ft_strlen(str);
	while (str[i] != '\0')
	{
		if (str[i++] == '\"')
			break ;
	}
	while (str[j] != '\0')
	{
		if (str[j--] == '\"')
			break ;
	}
	if (i < j)
	{
		while (i < j)
		{
			if (str[i] == '\"')
				str[i] = 15;
			if (str[i] == '\'')
				str[i] = 16;
			i++;
		}
	}
}

void	change_qoutes(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] != '\0')
	{
		if (str[i] == '\"' && i != 0 && i != (int)ft_strlen(str) - 1)
			str[i] = 15;
		if (str[i] == '\'' && i != 0 && i != (int)ft_strlen(str) - 1)
			str[i] = 16;
		i++;
	}
}

int	double_quotes(const char *str)
{
	int (in_double_quotes), (i);
	i = 0;
	in_double_quotes = 0;
	while (str[i] != '\0' && str[i] != '"')
	{
		if (str[i++] == '$')
			return (0);
	}
	while (str[i] != '\0')
	{
		if (str[i] == '"')
			in_double_quotes = 1;
		else if (str[i] == '$')
		{
			in_double_quotes = 2;
			break ;
		}
		i++;
	}
	while (str[i] != '\0')
		if (str[i++] == '\"' && in_double_quotes == 2)
			return (1);
	return (0);
}

void	process_arguments(t_command *current_command, t_exec_context *context)
{
	char	**args;
	char	*last;

	int (i), (j), (k);
	args = current_command->args;
	i = 0;
	while (args[i] != NULL)
	{
		last = args[i];
		k = 0;
		while (last && dstrchr(last, '$', &j) && !is_dollar_only(last))
		{
			handle_expansion(args, i, context, &k);
			if (should_split_argument(args, i, last))
			{
				handle_splitting(&args, i, context->gc);
				break ;
			}
			if (k >= (int)ft_strlen(args[i]))
				break ;
			last = args[i] + k;
		}
		i++;
	}
	current_command->args = args;
}
