/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:20:41 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:52:22 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_freespace(char **str, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(str[i++]);
	free(str);
	return (NULL);
}

void	ft_word_len2(char *str, int *i, int *wordlen)
{
	(*i)++;
	(*wordlen)++;
	if (str[*i - 1] == '\'')
	{
		while (str && str[*i] != '\'')
		{
			(*wordlen)++;
			(*i)++;
		}
	}
	else
	{
		while (str && str[*i] != '\"')
		{
			(*wordlen)++;
			(*i)++;
		}
	}
	(*wordlen)++;
	(*i)++;
}

int	ft_word_len(char *s, int i)
{
	int	wordlen;

	wordlen = 0;
	if (s[i] == '\'' || s[i] == '\"')
		ft_word_len2(s, &i, &wordlen);
	else
	{
		while (s[i] && s[i] != ' ' && !(s[i] >= 9 && s[i] <= 13))
		{
			if (s[i] == '\'' || s[i] == '\"')
				ft_word_len2(s, &i, &wordlen);
			else
			{
				i++;
				wordlen++;
			}
		}
	}
	if (s[i] && !(s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		wordlen += ft_word_len(s, i);
	return (wordlen);
}

char	**ft_splitall(char **str, char const *s, int countword,
		t_MemoryManager *gc)
{
	int	i;
	int	j;
	int	wordlen;

	i = 0;
	j = 0;
	while (s[i] && j < countword)
	{
		while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
			i++;
		wordlen = ft_word_len((char *)s, i);
		str[j] = gc_malloc(gc, (wordlen + 1) * sizeof(char));
		if (!str[j])
			return (ft_freespace(str, countword));
		set_word(str[j], s, &i);
		j++;
	}
	str[j] = NULL;
	return (str);
}

char	**ft_split(char const *s, t_MemoryManager *gc)
{
	int		countword;
	char	**str;

	if (!s)
		return (NULL);
	countword = ft_wordcount(s);
	str = gc_malloc(gc, (countword + 2) * sizeof(char *));
	if (!str)
		return (NULL);
	ft_splitall(str, s, countword, gc);
	return (str);
}
