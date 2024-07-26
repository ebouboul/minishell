/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:43:54 by ebouboul          #+#    #+#             */
/*   Updated: 2023/11/03 17:43:56 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		count;
	char	*str;

	str = (char *)s;
	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == (unsigned char)c)
			count = i;
		i++;
	}
	if (str[i] == (unsigned char)c)
		return (str = &str[i]);
	else if (count == 0 && s[0] != (unsigned char)c)
		return (0);
	else
	{
		str = &str[count];
		return (str);
	}
}
