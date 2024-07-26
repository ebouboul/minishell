#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>

int check_syntax(char *line)
{
	int i = 0;
	int count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ';')
		{
			count++;
		}
		i++;
	}
	if (count > 1)
	{
		return (1);
	}
	return (0);
}
int check_quote(char *line)
{
	int i = 0;
	int count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"')
		{
			count++;
		}
		i++;
	}
	if (count % 2 != 0)
	{
		return (1);
	}
	return (0);
}
int check_pipe(char *line)
{
	if (line[0] == '|' || line[strlen(line) - 1] == '|')
		{
			return (1);
		}
	return (0);
}


int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
    char *rl;
	while(1)
	{
		rl = readline("minishell > ");
		add_history(rl);
		if (strcmp(rl, "exit") == 0)
		{
			free(rl);
			break;
		}
		if (check_syntax(rl) == 1 || check_quote(rl) == 1 || check_pipe(rl) == 1)
		{
			printf("Syntax error\n");
			free(rl);
			continue;
		}
		char *token = strtok(rl, " ");
		while (token != NULL)
		{
			printf("%s\n", token);
			token = strtok(NULL, ";");
		}

		// printf("%s\n", rl);
		free(rl);
	
	}

    return (0);
}