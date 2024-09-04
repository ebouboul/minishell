

#include "minishell.h"

char* check_value_env(char *str, t_env *head)
{
    t_env *current;
    current = head;
    char *env_name;
    char *val;
    int i;
    i = 0;

    while (str[i] && (isalpha(str[i]) || isdigit(str[i]) || str[i] == '_'))
        i++;
    env_name = ft_substr(str, 1, i - 1);
    while (current != NULL)
    {
        if (strcmp(current->env->key, env_name) == 0)
        {
            val = current->env->value;
            gc_free(env_name);
            return val;
        }
        current = current->next;
    }

    gc_free(env_name);
    return "";
}
int ft_strlen1(char **str)
{
    int i = 0;
    while (str[i] != NULL)
    {
        i++;
    }
    return i;
}




char **resize_args(char **args, int new_size)
{
    char **new_args = (char **)gc_malloc(sizeof(char *) * (new_size + 1));
    int i = 0;

    if (new_args == NULL)
    {
        perror("Memory allocation failed\n");
        return NULL;
    }

    while (args[i] != NULL)
    {
        new_args[i] = args[i];
        i++;
    }

    new_args[i] = NULL;
    gc_free(args);
    return new_args;
}
int dollar_position(char *str)
{
    int i = 1;
    while (str[i] != '\0')
    {
        if (str[i] == '$')
            return i;
        i++;
    }
    if(str[0] == '$')
        return 1;
    return -1;
}

// void expansion_process(t_node **head, t_env *env_list)
// {
//     t_node *current = *head;
//     t_command *current_command;
//     char **args;
//     char **split_args;
//     char *value;
//     char *new_arg;
//     char *temp;
//     int i, j, k, num_splits, original_size;

//     while (current != NULL)
//     {
//         current_command = current->command;
//         while (current_command != NULL)
//         {
//             args = current_command->args;
//             i = 0;
//             while (args[i] != NULL)
//             {
//                 if (ft_strchr(args[i], '$') == NULL)
//                 {
//                     i++;
//                     continue;
//                 }
//                 new_arg = ft_strdup(""); // Start with an empty string
//                 j = 0;
//                 while (args[i][j] != '\0')
//                 {
//                     if (args[i][j] == '$' && ( j == 0||(j != 0 && args[i][j - 1] != '\'' )) && args[i][j + 1] != '\0' && (isalpha(args[i][j + 1]) 
//                         || args[i][j + 1] == '_' || args[i][j + 1] == '?' || args[i][j + 1] == '$'))
//                     {
//                         if(args[i][j + 1] == '?')
//                         {
//                             value = ft_itoa(current->exit_status);
//                             temp = new_arg;
//                             new_arg = ft_strjoin(temp, value);
//                             j++;
//                             gc_free(temp);
//                             j++;
//                             continue;
//                         }
//                         else
//                         {

//                         k = j;
//                         while (args[i][k] && (isalpha(args[i][k + 1]) || isdigit(args[i][k + 1]) || args[i][k + 1] == '_'))
//                             k++;
//                         // Get the value of the environment variable
//                         value = check_value_env(ft_substr(args[i], j, k - j + 1), env_list);
//                         temp = new_arg;
//                         new_arg = ft_strjoin(temp, value);
//                         gc_free(temp);
//                         // Skip over the variable name in the original string
//                         while (args[i][j] && (isalpha(args[i][j + 1]) || isdigit(args[i][j + 1]) || args[i][j + 1] == '_'))
//                             j++;
//                         }
//                     }
//                     else 
//                     {
//                         // Append the current character if it's not part of an environment variable
//                         temp = new_arg;
//                         new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));
//                         gc_free(temp);
//                     }
//                     j++;
//                 }
                
//                 remove_quotes_from_first_and_last(new_arg);
//                 if(new_arg[0] == '\0')
//                 {
//                     args[i] = new_arg;
//                     i++;
//                     continue;
//                 }
                
//                 // Handle case where args[i] contains quotes and needs to be split
//                 if(args[i][dollar_position(args[i]) - 1] == '"' && ft_strchr(args[i] + dollar_position(args[i] + 1), '"') && new_arg[0] != '\0' )
//                 {
//                     args[i] = new_arg; // Replace with the expanded string
//                     remove_all_quotes_and_join(args[i]);
//                     i++;
//                 }
//                 else if(new_arg[0] != '\0' && new_arg && new_arg[0] != ' ')
//                 {
//                     split_args = ft_split3(new_arg, ' ');
//                     gc_free(new_arg);
//                     if (split_args[0] != NULL)
//                     {
//                     // Calculate the size of the args array
//                     original_size = 0;
//                     while (args[original_size] != NULL)
//                         original_size++;

//                     // Calculate the number of splits
//                     num_splits = 0;
//                     while (split_args[num_splits] != NULL)
//                         num_splits++;

//                     // Resize the args array to accommodate new arguments
//                     args = resize_args(args, original_size + num_splits - 1);

//                     // Insert the split arguments into the args array
//                     k = original_size + num_splits - 1;
//                     while (k >= i + num_splits)
//                     {
//                         args[k] = args[k - num_splits + 1];
//                         k--;
//                     }

//                     k = 0;
//                     while (split_args[k] != NULL)
//                     {
//                         args[i + k] = split_args[k];
//                         remove_all_quotes_and_join(args[i + k]);
//                         k++;
//                     }
//                     gc_free(split_args);
//                     i += k;
//                     }
//                 }
                
//             }
//             current_command->args = args;
//             current_command = current_command->next;
//         }
//         current = current->next;
//     }
// }
void remove_args(char **args, int i)
{
    int j = i;
    while (args[j] != NULL)
    {
        args[j] = args[j + 1];
        j++;
    }
}

void handle_expansion(char **args, int i, t_env *env_list, int exit_status)
{
    char *new_arg = ft_strdup("");
    char *temp, *value;
    int j = 0, k;

    while (args[i][j] != '\0')
    {
        if (args[i][j] == '$' && (j == 0 || (j != 0 && args[i][j - 1] != '\'')) && args[i][j + 1] != '\0' &&
            (isalpha(args[i][j + 1]) || args[i][j + 1] == '_' || args[i][j + 1] == '?' || args[i][j + 1] == '$'))
        {
            if (args[i][j + 1] == '?')
            {
                value = ft_itoa(exit_status);
                temp = new_arg;
                new_arg = ft_strjoin(temp, value);
                gc_free(temp);
                j += 2;
                continue;
            }
            k = j;
            while (args[i][k] && (isalpha(args[i][k + 1]) || isdigit(args[i][k + 1]) || args[i][k + 1] == '_'))
                k++;
            value = check_value_env(ft_substr(args[i], j, k - j + 1), env_list);
            temp = new_arg;
            new_arg = ft_strjoin(temp, value);
            gc_free(temp);
            j = k + 1;
        }
        else
        {
            temp = new_arg;
            new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));
            gc_free(temp);
            j++;
        }
    }
    if(new_arg[0] != '\0' )
    {
        args[i] = new_arg;
        return;
    }
    else
    remove_args(args, i);
}

void handle_splitting(char ***args, int i)
{
    char **split_args = ft_split3((*args)[i], ' ');
    int original_size = 0, num_splits = 0, k;

    while ((*args)[original_size] != NULL)
        original_size++;
    while (split_args[num_splits] != NULL)
        num_splits++;

    *args = resize_args(*args, original_size + num_splits - 1);

    k = original_size + num_splits - 1;
    while (k >= i + num_splits)
    {
        (*args)[k] = (*args)[k - num_splits + 1];
        k--;
    }

    for (k = 0; k < num_splits; k++)
    {
        (*args)[i + k] = split_args[k];
        remove_all_quotes_and_join((*args)[i + k]);
    }
    gc_free(split_args);
}

void process_arguments(t_command *current_command, t_env *env_list, int exit_status)
{
    char **args = current_command->args;
    int i = 0;

    while (args[i] != NULL)
    {
        if (ft_strchr(args[i], '$') != NULL)
        {
            handle_expansion(args, i, env_list, exit_status);
            if (args[i] && args[i][0] != '"' && ft_strchr(args[i], ' ') != NULL && ft_strncmp(args[0], "export", 6) != 0)
                handle_splitting(&args, i);
        if(args[i])
        remove_quotes_from_first_and_last(args[i]);
        }
        i++;
    }
    current_command->args = args;
}
void expansion_redirection(t_command *red, t_env *env)
{
    while(red->redirect)
    {
        if(ft_strchr(red->redirect->str, '$') != NULL)
        {
            char *new_arg = ft_strdup("");
            char *temp, *value;
            int j = 0, k;
            while (red->redirect->str[j] != '\0')
            {
                if (red->redirect->str[j] == '$' && (j == 0 || (j != 0 && red->redirect->str[j - 1] != '\'')) && red->redirect->str[j + 1] != '\0' &&
                    (isalpha(red->redirect->str[j + 1]) || red->redirect->str[j + 1] == '_' || red->redirect->str[j + 1] == '?' || red->redirect->str[j + 1] == '$'))
                {
                    
                    k = j;
                    while (red->redirect->str[k] && (isalpha(red->redirect->str[k + 1]) || isdigit(red->redirect->str[k + 1]) || red->redirect->str[k + 1] == '_'))
                        k++;
                    value = check_value_env(ft_substr(red->redirect->str, j, k - j + 1), env);
                    temp = new_arg;
                    new_arg = ft_strjoin(temp, value);
                    gc_free(temp);
                    j = k + 1;
                }
                else
                {
                    temp = new_arg;
                    new_arg = ft_strjoin(temp, ft_substr(red->redirect->str, j, 1));
                    gc_free(temp);
                    j++;
                }
            }
            if(new_arg[0] != '\0' )
            {
                red->redirect->str = new_arg;
            }
            else
            {
                red->redirect = red->redirect->next;
            }
        }
        else
        {
            red->redirect = red->redirect->next;
        }
    }
}

void expansion_process(t_node **head, t_env *env_list)
{
    t_node *current = *head;

    while (current != NULL)
    {
        t_command *current_command = current->command;
        while (current_command != NULL)
        {
            process_arguments(current_command, env_list, current->exit_status);
            // expansion_redirection(current_command, env_list);

            current_command = current_command->next;
        }
        current = current->next;
    }
}

