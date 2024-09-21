/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:50:40 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:55:05 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*gc_realloc(t_MemoryManager *manager, void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (gc_malloc(manager, new_size));
	new_ptr = gc_malloc(manager, new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	gc_free(manager, ptr);
	return (new_ptr);
}
