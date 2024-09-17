/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:47:16 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 02:25:20 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(MemoryManager *manger, const char *src)
{
	char	*dest;
	int		i;

	i = 0;
	dest = (char *)gc_malloc(manger, (ft_strlen(src) + 1));
	if (!dest)
	{
		perror("Memory allocation failed\n");
		exit(1);
	}
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
