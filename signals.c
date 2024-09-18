#include "minishell.h"

void handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		write(1, "minishell$ ", 11);
	}
}
void handler_c(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		// rl_replace_line("", 0);
		// // rl_on_new_line();
		// // rl_redisplay();
		exit(130);
	}
}

