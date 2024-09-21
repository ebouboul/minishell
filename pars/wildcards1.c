/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:33:37 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:54:49 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

DIR	*open_directory(char *path)
{
	DIR	*dir;

	dir = opendir(".");
	if (dir == NULL)
	{
		print_error11("opendir", path);
		return (NULL);
	}
	return (dir);
}

void	collect_matches(DIR *dir, char *path, t_replace_wildcard_vars *vars,
		t_MemoryManager *manager)
{
	struct dirent	*entry;

	entry = readdir(dir);
	while (entry != NULL)
	{
		if (wildcard_match(path, entry->d_name))
		{
			vars->match_count++;
			vars->matches = gc_realloc(manager, vars->matches,
					(vars->match_count) * sizeof(char *));
			vars->matches[vars->match_count - 1] = ft_strdup(manager,
					entry->d_name);
		}
		entry = readdir(dir);
	}
}

void	insert_wildcard_matches(char ***args, int i,
		t_replace_wildcard_vars *vars, t_MemoryManager *manager)
{
	int	j;

	vars->old_size = ft_strlen1(*args);
	vars->new_size = vars->old_size + vars->match_count - 1;
	*args = gc_realloc(manager, *args, (vars->new_size + 1) * sizeof(char *));
	shift_args(*args, i, vars->old_size, vars->match_count);
	j = 0;
	while (j < vars->match_count)
	{
		(*args)[i + j] = vars->matches[j];
		j++;
	}
}

void	replace_wildcard(char ***args, int *i, t_MemoryManager *manager)
{
	t_replace_wildcard_vars	vars;
	DIR						*dir;

	vars.path = (*args)[*i];
	dir = open_directory(vars.path);
	if (dir == NULL)
		return ;
	vars.match_count = 0;
	vars.matches = NULL;
	collect_matches(dir, vars.path, &vars, manager);
	closedir(dir);
	if (vars.match_count == 0)
	{
		gc_free(manager, vars.matches);
		return ;
	}
	insert_wildcard_matches(args, *i, &vars, manager);
	gc_free(manager, vars.matches);
}

void	replace_wildcard_in_args(t_node *head, t_MemoryManager *manager)
{
	t_node	*current;
	int		i;

	current = head;
	while (current != NULL)
	{
		if (is_wildcard_in_args(current->command->args))
		{
			i = 0;
			while (current->command->args[i] != NULL)
			{
				if (is_wildcard(current->command->args[i]))
				{
					replace_wildcard(&current->command->args, &i, manager);
				}
				i++;
			}
		}
		current = current->next;
	}
}
