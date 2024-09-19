/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:44:02 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 03:08:05 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countword(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	if (!s)
		return (0);
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static char	**free_tab(char **tab, size_t j, MemoryManager *gc)
{
	size_t	i;

	i = 0;
	while (i < j)
	{
		gc_free(gc, tab[i]);
		i++;
	}
	gc_free(gc, tab);
	return (NULL);
}

char	find_dilm(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			return (str[i]);
		i++;
	}
	return (0);
}

char	**ft_split3(char const *s, char c, MemoryManager *gc)
{
	char	**tab;

	int (i), (j), (k);
	if (find_dilm((char *)s) != 0)
		c = find_dilm((char *)s);
	tab = (char **)gc_malloc(gc, sizeof(char *) * (ft_countword(s, c) + 1));
	if (!s || !tab)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < (int)ft_countword(s, c))
	{
		while (s[i] == c)
			i++;
		k = i;
		while (s[i] != c && s[i])
			i++;
		tab[j] = ft_substr(s, k, i - k, gc);
		if (i > k && !(tab[j]))
			return (free_tab(tab, j, gc));
		j++;
	}
	tab[j] = NULL;
	return (tab);
}
