/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:47:25 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:51:50 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**resize_args(char **args, int new_size, t_MemoryManager *gc)
{
	char	**new_args;
	int		i;

	new_args = (char **)gc_malloc(gc, (new_size + 1) * sizeof(char *));
	i = 0;
	while (i < new_size)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = NULL;
	return (new_args);
}

void	remove_args(char **args, int i)
{
	int	j;

	j = i;
	while (args[j] != NULL)
	{
		if (j + 1 == ft_strlen1(args))
		{
			args[j] = NULL;
			break ;
		}
		else
			args[j] = args[j + 1];
		j++;
	}
}

int	in_quotes(char *str, int i, char c)
{
	int	flag;

	flag = 0;
	while (i >= 0)
	{
		if (str[i] == '\"')
		{
			i--;
			while (i >= 0 && str[i] != '\"')
			{
				if (str[i] == c)
					flag = 1;
				i--;
			}
			if (i < 0 && flag == 1)
				return (0);
			else
				i--;
			flag = 0;
		}
		else
			i--;
	}
	return (1);
}

int	dstrchr2(char *s, char c, int *i)
{
	char	quote;

	(*i)++;
	quote = s[*i - 1];
	while (s[*i] && s[*i] != quote)
	{
		if (s[*i] == c && s[*i + 1] != '\0' && quote == '\"')
			return (1);
		(*i)++;
	}
	if (s[*i] == quote)
		(*i)++;
	return (0);
}

char	*dstrchr(char *s, char c, int *flag)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if ((s[i] == '\"' && ft_strchr(&s[i + 1], '\"')) || s[i] == '\''
			|| !in_quotes(s, (ft_strlen(s) - 1), c))
		{
			if (s[i] != '\"' && s[i] != '\'')
			{
				while (s[i] && s[i] != c && s[i] != '\"')
					i++;
				if (s[i] == c)
					return (*flag = 1, &s[i]);
			}
			else if (dstrchr2(s, c, &i))
				return (*flag = 1, &s[i]);
		}
		else if (s[i] == c)
			return (*flag = i, &s[i]);
		else
			i++;
	}
	return (NULL);
}
