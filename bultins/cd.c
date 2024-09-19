/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:38:26 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 20:11:14 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_path(char *path, t_env **env_list, MemoryManager *manager)
{
	if (ft_strcmp(path, "-") == 0)
	{
		return (get_oldpwd_path(env_list));
	}
	else if (path[0] == '~')
	{
		return (handle_home_shortcut(path, env_list, manager));
	}
	return (path);
}

char	*get_home_path(t_env **env_list)
{
	char	*path;

	path = get_env_value(*env_list, "HOME");
	if (path == NULL)
	{
		printf("cd: HOME not set\n");
	}
	return (path);
}

char	*get_path(t_command *command, t_env **env_list, MemoryManager *manager)
{
	if (command->args[1] == NULL)
	{
		return (get_home_path(env_list));
	}
	else
	{
		return (process_path(command->args[1], env_list, manager));
	}
}

void	update_env(t_env **env_list, char *oldpwd, MemoryManager *manager)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	replace_env_value(*env_list, "OLDPWD", oldpwd, manager);
	replace_env_value(*env_list, "PWD", pwd, manager);
	gc_free(manager, pwd);
}

int	ft_cd(t_command *command, t_env **env_list, MemoryManager *manager)
{
	int		status;
	char	*path;
	char	*oldpwd;

	if (command->args[1] && command->args[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	path = get_path(command, env_list, manager);
	if (path == NULL)
		return (1);
	oldpwd = getcwd(NULL, 0);
	status = chdir(path);
	if (status != -1)
		update_env(env_list, oldpwd, manager);
	else
	{
		printf("cd: %s: %s\n", path, strerror(errno));
		gc_free(manager, oldpwd);
		return (1);
	}
	gc_free(manager, oldpwd);
	return (0);
}
