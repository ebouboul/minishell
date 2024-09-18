#include "minishell.h"

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
void handler_c(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	exit(130);
	
}

