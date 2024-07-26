/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:42:52 by ebouboul          #+#    #+#             */
/*   Updated: 2023/11/03 17:42:54 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	lendst;
	size_t	lensrc;

	lensrc = ft_strlen(src);
	lendst = ft_strlen(dst);
	i = 0;
	if (size == 0 && !dst)
		return (lensrc);
	if (size != 0)
	{
		while (i + lendst < size - 1 && src[i] != '\0')
		{
			dst[lendst + i] = src[i];
			i++;
		}
	}
	dst[lendst + i] = '\0';
	if (size > lendst)
		return (lendst + lensrc);
	else
		return (size + lensrc);
}
