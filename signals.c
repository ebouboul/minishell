#include "minishell.h"


int		globat_status;

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		globat_status = 130;
	}
}

void handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
void	ig_signal(t_node *m, int i)
{
	if (i == 1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (i == 3)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
	}
	if (globat_status == 130)
		m->exit_status = 130;
	globat_status = 0;		
}


void handler_c(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	exit(130);
	
}

void	sig_i_herdoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

void	sig_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	sig_herdoc(void)
{
	signal(SIGINT, sig_i_herdoc);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_ignor(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}


