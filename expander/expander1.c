/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:45:36 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/19 19:45:45 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_args(char **args, char *new_arg, int i)
{
	if (new_arg[0] != '\0')
		args[i] = new_arg;
	else
		remove_args(args, i);
}

int	get_arg_size(char **args)
{
	int	size;

	size = 0;
	while (args[size] != NULL)
		size++;
	return (size);
}

void	shift_args(char **args, int i, int original_size, int num_splits)
{
	int	k;

	k = original_size + num_splits - 1;
	while (k >= i + num_splits)
	{
		args[k] = args[k - num_splits + 1];
		k--;
	}
}

void	split_and_remove_quotes(char ***args, char **split_args, int i,
		int num_splits)
{
	int	k;

	k = 0;
	while (k < num_splits)
	{
		(*args)[i + k] = split_args[k];
		// remove_quotes_from_first_and_last_only((*args)[i + k]);
		k++;
	}
}

void	handle_splitting(char ***args, int i, MemoryManager *gc)
{
	char **split_args;
	int num_splits;
	int original_size;

	if (ft_strchr((*args)[i], '"') != NULL && ft_strchr((*args)[i],
			'\'') != NULL)
		split_args = ft_split((*args)[i], gc);
	else
		split_args = ft_split3((*args)[i], ' ', gc);
	original_size = get_arg_size(*args);
	num_splits = get_arg_size(split_args);
	*args = resize_args(*args, original_size + num_splits - 1, gc);
	shift_args(*args, i, original_size, num_splits);
	split_and_remove_quotes(args, split_args, i, num_splits);
	gc_free(gc, split_args);
}