/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirectionsP2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:37:10 by ansoulai          #+#    #+#             */
/*   Updated: 2024/09/20 00:28:10 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NORM=OK!
void	handle_single_redirection(t_redirect *redirect)
{
	if (redirect->flag == 4)
		redirect_output(redirect->str, O_WRONLY | O_CREAT | O_TRUNC);
	else if (redirect->flag == 5)
		redirect_output(redirect->str, O_WRONLY | O_CREAT | O_APPEND);
	else if (redirect->flag == 3)
		redirect_input(redirect->str);
}

int	is_empty_in_qotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			i++;
			break ;
		}
		i++;
	}
	while (str[i] && str[i] != '"' && str[i] != '\'')
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\v'
			&& str[i] != '\f' && str[i] != '\r')
			return (1);
		i++;
	}
	return (0);
}

int	ambigous_redirect(char *str, char *old)
{
	int	flag;

	if ((ft_strchr(str, '"') || ft_strchr(str, '\'')) && is_empty_in_qotes(str))
		flag = 1;
	else
		flag = 0;
	remove_closed(str);
	if ((ft_strchr(str, ' ') || ft_strchr(str, '\t') || ft_strchr(str, '\n')
			|| ft_strchr(str, '\v') || ft_strchr(str, '\f') || ft_strchr(str,
				'\r') || ft_strlen(str) == 0) && flag == 0)
	{
		print_error11(old, "ambiguous redirect");
		return (1);
	}
	return (0);
}

int	handle_redirections(t_node *node, t_env **env_list, int *exit_status)
{
	t_redirect	*redirect;

	redirect = node->command->redirect;
	(void)env_list;
	(void)exit_status;
	while (redirect)
	{
		if (redirect->flag != 8)
		{
			if (ambigous_redirect(redirect->str, redirect->old_str) == 0)
				handle_single_redirection(redirect);
			else
				return (1);
		}
		redirect = redirect->next;
	}
	return (0);
}

void	wait_for_children(pid_t last_pid, int *exit_status)
{
	int	i;
	int	status;

	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			*exit_status = 128 + WTERMSIG(status);
	}
	i = wait(NULL);
	while (i > 0)
		i = wait(NULL);
}
