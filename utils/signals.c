/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:55:25 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/20 02:44:34 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		g_lobat_status;

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_lobat_status = 130;
	}
}

void	sig_child(void)
{
	signal(SIGINT, sig_herdoc);
	signal(SIGQUIT, SIG_DFL);
}

void	sig_herdoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
}

void	sig_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sigoo(t_node *m)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	if (g_lobat_status == 130)
		m->exit_status = 130;
	g_lobat_status = 0;
}
