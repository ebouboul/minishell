#include "minishell.h"


int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1 && s2 && s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

int is_builtin(char *command)
{
    if (ft_strcmp(command, "echo") == 0)
        return 1;
    else if (ft_strcmp(command, "cd") == 0)
        return 1;
    else if (ft_strcmp(command, "pwd") == 0)
        return 1;
    else if (ft_strcmp(command, "export") == 0)
        return 1;
    else if (ft_strcmp(command, "unset") == 0)
        return 1;
    else if (ft_strcmp(command, "env") == 0)
        return 1;
    else if (ft_strcmp(command, "exit") == 0)
        return 1;
    return 0;
}

int process_echo_option(char *option)
{
    int i = 1;
    while (option[i] != '\0')
    {
        if (option[i] != 'n')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int ft_echo(t_command *command)
{
    int option_n = 0;
    int i = 1;
    int first_arg = 1;

    while (command->args[i] != NULL && command->args[i][0] == '-' && command->args[i][1] == 'n')
    {
        if (process_echo_option(command->args[i]))
        {
            option_n = 1;
            i++;
        }
        else
            break;
    }
    while (command->args[i] != NULL)
    {
        if (!first_arg)
            printf(" ");
        printf("%s", command->args[i]);
        first_arg = 0;
        i++;
    }
    if (!option_n)
        printf("\n");
    return 0;
}
int check_key_from_env(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

char *get_env_value(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
            return current->env->value;
        current = current->next;
    }
    return NULL;
}

void replace_env_value(t_env *env_list, char *key, char *value, MemoryManager *manager)
{
    t_env *current = env_list;
    if (value == NULL)
        return;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
        {
            gc_free(manager, current->env->value);
            current->env->value = ft_strdup(manager, value);
            break;
        }
        current = current->next;
    }
}

int ft_cd(t_command *command, t_env **env_list, MemoryManager *manager)
{
    if(command->args[1] && command->args[2])
    {
        printf("cd: too many arguments\n");
        return 1;
    }
    char *path;
    if (command->args[1] == NULL)
    {
        path = get_env_value(*env_list, "HOME");
        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    }
    else
    {
        path = command->args[1];
        if (ft_strcmp(path, "-") == 0)
        {
            path = get_env_value(*env_list, "OLDPWD");
            if (path == NULL)
            {
                printf("cd: OLDPWD not set\n");
                return 1;
            }
        }
        else if (path[0] == '~')
        {
            char *home = get_env_value(*env_list, "HOME");
            if (home == NULL)
            {
                printf("cd: HOME not set\n");
                return 1;
            }
            path = ft_strjoin(home, path + 1, manager);
        }
    }
    char *oldpwd = getcwd(NULL, 0);
    int status = chdir(path);
    if(status != -1)
    {
        char *pwd = getcwd(NULL, 0);
        replace_env_value(*env_list, "OLDPWD", oldpwd, manager);
        replace_env_value(*env_list, "PWD", pwd, manager);
        gc_free(manager, oldpwd);
        gc_free(manager, pwd);
    }
    else
    {
        printf("cd: %s: %s\n", path, strerror(1));
        gc_free(manager, oldpwd);
        return 1;
    }
    return 0;
}

int ft_pwd()
{
    char *path = getcwd(NULL, 0);
    if (path == NULL)
    {
        printf("pwd: %s\n", strerror(1));
        return 1;
    }
    printf("%s\n", path);
    free(path);
    return 0;
}

int ft_env(t_env *env_list)
{
    print_env_list(env_list);
    return 0;
}
int is_numeric(const char *str)
{
    int i = 0;
    while (str &&str[i] != '\0')
    {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
            return 0;
        i++;
    }
    return 1;
}

int ft_exit(t_command *command, MemoryManager *manager)
{
    int status = 0;
    if (ft_strlen1(command->args) > 2 && !is_numeric(command->args[1]))
    {
        if (is_numeric(command->args[1]))
            printf("exit: %s: numeric argument required\n", command->args[1]);
        else
        {
            printf("exit: too many arguments\n");
            return 1;
        }
    }
    if (command->args[1] != NULL)
    {
        status = ft_atoi(command->args[1]);
    }
    if (!is_numeric(command->args[1]))
    {
        printf("exit: %s: numeric argument required\n", command->args[1]);
        return 1;
    }
    gc_free_all(manager);
    exit(status);
}

int execute_builtin(t_node *head, t_env **env_list, MemoryManager *manager)
{
    t_node *current = head;
    int status = 0;
    if (ft_strcmp(current->command->args[0], "echo") == 0)
        status = ft_echo(current->command);
    else if (ft_strcmp(current->command->args[0], "cd") == 0)
        status = ft_cd(current->command, env_list, manager);
    else if (ft_strcmp(current->command->args[0], "pwd") == 0)
        status = ft_pwd();
    else if (ft_strcmp(current->command->args[0], "export") == 0)
        status = ft_export(current->command, env_list, manager);
    else if (ft_strcmp(current->command->args[0], "unset") == 0)
        status = ft_unset(current->command, env_list, manager);
    else if (ft_strcmp(current->command->args[0], "env") == 0)
        status = ft_env(*env_list);
    else if (ft_strcmp(current->command->args[0], "exit") == 0)
        status = ft_exit(current->command, manager);
    return status;
}
