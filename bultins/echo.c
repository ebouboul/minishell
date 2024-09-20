/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amousaid <amousaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:46:53 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/20 21:11:31 by amousaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_echo_option(char *option)
{
	int	i;

	i = 1;
	while (option[i] != '\0')
	{
		if (option[i] != 'n')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_echo(t_command *command)
{
	int (option_n), (i), (first_arg);
	option_n = 0;
	i = 1;
	first_arg = 1;
	while (command->args[i] != NULL && command->args[i][0] == '-'
		&& command->args[i][1] == 'n')
	{
		if (process_echo_option(command->args[i]))
		{
			option_n = 1;
			i++;
		}
		else
			break ;
	}
	while (command->args[i] != NULL)
	{
		if (!first_arg)
			printf(" ");
		printf("%s", command->args[i++]);
		first_arg = 0;
	}
	if (!option_n)
		printf("\n");
	return (0);
}

char	*handle_home_shortcut(char *path, t_env **env_list,
		MemoryManager *manager)
{
	char	*home;

	home = get_env_value(*env_list, "HOME");
	if (home == NULL)
	{
		print_error11("cd", "HOME not set");
		return (NULL);
	}
	return (ft_strjoin(home, path + 1, manager));
}

char	*get_oldpwd_path(t_env **env_list)
{
	char	*path;

	path = get_env_value(*env_list, "OLDPWD");
	if (path == NULL)
		print_error11("cd", "OLDPWD not set");
	return (path);
}
