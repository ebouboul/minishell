/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:59:36 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 01:49:22 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i] != '\0')
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
			return (0);
		i++;
	}
	return (1);
}

int	is_all_numeric(char **str)
{
	int	i;

	i = 1;
	while (str && str[i] != NULL)
	{
		if (!is_numeric(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_command *command, t_MemoryManager *manager)
{
	int	status;

	status = 0;
	if (ft_strlen1(command->args) > 2 && is_all_numeric(command->args))
	{
		print_error11(NULL, "too many arguments");
		return (1);
	}
	else if (command->args[1] != NULL)
		status = ft_atoi(command->args[1]);
	printf("exit\n");
	if (!is_numeric(command->args[1]))
		print_error11(command->args[1], "numeric argument required");
	my_exit(status, manager);
	return (0);
}