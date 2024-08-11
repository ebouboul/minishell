/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:42:38 by ebouboul          #+#    #+#             */
/*   Updated: 2023/11/16 00:33:11 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	a;
	size_t	i;

	a = 0;
	if (!s)
		return (NULL);
	i = ft_strlen(s);
	if (start > i)
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (a < len)
	{
		str[a] = s[start + a];
		a++;
	}
	return (str);
}
