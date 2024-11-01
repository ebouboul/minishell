/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_space_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:11:28 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/26 20:30:48 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_extra_spaces(char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (is_special_char2(input[i]))
		{
			if ((input[i] == '>' && input[i + 1] == '>')
				|| (input[i] == '<' && input[i + 1] == '<'))
			{
				if (i == 0 || input[i - 1] != ' '
					|| input[i + 2] != ' ')
					j += 2;
				i++;
			}
			else if (i == 0 || input[i - 1] != ' ' || input[i + 1] != ' ')
				j++;
		}
		i++;
	}
	return (j);
}

void	add_space_around_double_char(char *input, char *new, int *i, int *j)
{
	if (*j == 0 || new[*j - 1] != ' ')
		new[(*j)++] = ' ';
	new[(*j)++] = input[(*i)++];
	new[(*j)++] = input[(*i)++];
	if (input[*i] != ' ')
		new[(*j)++] = ' ';
}

void	add_space_around_single_char(char *input, char *new, int *i, int *j)
{
	if (*j == 0 || new[*j - 1] != ' ')
		new[(*j)++] = ' ';
	new[(*j)++] = input[(*i)++];
	if (input[*i] != ' ')
		new[(*j)++] = ' ';
}

char	*add_spaces(char *input, t_MemoryManager *manager)
{
	int		i;
	int		j;
	int		length;
	char	*new;

	length = ft_strlen(input) + (count_extra_spaces(input) * 2) + 1;
	new = (char *)gc_malloc(manager, length);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
			add_space_around_double_char(input, new, &i, &j);
		else if (is_special_char2(input[i]))
			add_space_around_single_char(input, new, &i, &j);
		else
			new[j++] = input[i++];
	}
	new[j] = '\0';
	return (new);
}

void	increment_shlvl(t_env **env_list, t_MemoryManager *manager)
{
	t_env	*current;
	int		value;
	char	*new_value;

	current = *env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, "SHLVL") == 0)
		{
			value = ft_atoi(current->env->value);
			value++;
			new_value = ft_itoa(value, manager);
			gc_free(manager, current->env->value);
			current->env->value = new_value;
			break ;
		}
		current = current->next;
	}
}
