/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:44:37 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 19:44:42 by ebouboul         ###   ########.fr       */
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
	int	i;
	int	j;

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
	int(in_double_quotes), (i);
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

void	process_arguments(t_command *current_command, t_env *env_list,
		int exit_status, MemoryManager *gc)
{
	char(**args), (*last);
	int(i), (j), (k);
	args = current_command->args;
	i = 0;
	while (args[i] != NULL)
	{
		last = args[i];
		k = 0;
		while (last && dstrchr(last, '$', &j) && !is_dollar_only(last))
		{
			handle_expansion(args, i, env_list, exit_status, &k, gc);
			printf("last = %s\n", last);
			if (args[i] && ft_strchr(args[i], ' ') != NULL
				&& ft_strncmp(args[0], "export", 6) != 0
				&& is_last_dollar(args[i], '$') == 0
				&& double_quotes(last) == 0)
			{
				printf("args[i] = %s\n", args[i]);
				handle_splitting(&args, i, gc);
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