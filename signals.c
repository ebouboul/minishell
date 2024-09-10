#include "minishell.h"

void handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
	}
}

