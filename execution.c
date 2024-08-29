// #include "minishell.h"

// void execute_commands(t_node *head, t_env **env_list)
// {
// 	t_node *current = head;
// 	while (current != NULL)
// 	{
//     	execute_single_command(current, env_list);
//     	current = current->next;
// 	}
// }

// void execute_single_command(t_node *node, t_env **env_list)
// {
// 	if (node == NULL || node->command == NULL || node->command->args == NULL || node->command->args[0] == NULL)
//     	return;

// 	char *cmd = node->command->args[0];

// 	if (is_builtin(cmd))
//     	node->exit_status = execute_builtin(node, env_list);
// 	else
// 		// node->exit_status = execute_external(node->command, *env_list);
// }

// int check_env_for_path(t_env *env_list)
// {
// 	t_env *current = env_list;
// 	while (current != NULL)
// 	{
// 		if (strcmp(current->env->key, "PATH") == 0)
// 			return 1;
// 		current = current->next;
// 	}
// 	return 0;
// }

// char split_path_in_env(char *path, char **paths)
// {
// 	int i = 0;
// 	char *token = strtok(path, ":");
// 	while (token != NULL)
// 	{
// 		paths[i] = token;
// 		token = ft_split(":");
// 		i++;
// 	}
// 	return i;
// }
// char join_command_with_path_and_access(char *command, char **paths, int n)
// {
// 	int i = 0;
// 	while (i < n)
// 	{
// 		char *path = ft_strjoin(paths[i], command);
// 		if (access(path, F_OK) == 0)
// 			return path;
// 		i++;
// 	}
// 	return NULL;
// }
// void execute_external(t_command *command, t_env *env_list)
// {
// 	if (check_env_for_path(env_list) == 0)
// 	{
// 		printf("minishell: %s: command not found\n", command->args[0]);
// 		return;
// 	}
// 	char *path = check_value_env("PATH", env_list);
// 	char **paths = malloc(sizeof(char *) * 100);
// 	int n = split_path_in_env(path, paths);
// 	char *path_to_command = join_command_with_path_and_access(command->args[0], paths, n);
// 	if (path_to_command == NULL)
// 	{
// 		printf("minishell: %s: command not found\n", command->args[0]);
// 		return;
// 	}
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		execve(path_to_command, command->args);
// 		perror("execv");
// 		exit(1);
// 	}
// 	else if (pid > 0)
// 	{
// 		int status;
// 		waitpid(pid, &status, 0);
// 	}
// 	else
// 	{
// 		perror("fork");
// 	}
// }


// // access this path if its valid or not

// // exceve this path if its valid

// // split path in env with ':' and return the number of paths
// // Then join the command with each path and check with access function if the command exists
// // If it exists, return the path
// // If it doesn't exist, return NULL
// // If the command is not found in any path, return NULL
// // if the command is found in multiple paths, return the first path
// // char *get_path(char *command, t_env *env_list)
// // give the path of the command to exceve function



