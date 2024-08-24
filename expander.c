

#include "minishell.h"

char* check_value_env(char *str, t_env *head)
{
    t_env *current = head;
    char *val;
    int i = 0;

    // Determine the length of the environment variable name
    while (str[i] && (isalpha(str[i]) || isdigit(str[i]) || str[i] == '_'))
    {
        i++;
    }

    // Extract the environment variable name
    char *env_name = ft_substr(str, 1, i - 1);

    while (current != NULL)
    {
        if (strcmp(current->env->key, env_name) == 0)
        {
            val = current->env->value;
            free(env_name);
            return val;
        }
        current = current->next;
    }

    free(env_name);
    return "(null)";
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


// char **resize_args(char **args, int new_size)
// {
//     char **new_args = (char **)malloc(sizeof(char *) * (new_size + 1));
//     int i = 0;

//     if (new_args == NULL)
//     {
//         perror("Memory allocation failed\n");
//         exit(1);
//     }

//     while (args[i] != NULL)
//     {
//         new_args[i] = args[i];
//         i++;
//     }

//     new_args[i] = NULL;
//     free(args);
//     return new_args;
// }

// void expansion_process(t_node **head, t_env *env_list)
// {
//     t_node *current = *head;
//     t_command *current_command;
//     char **args;
//     char **split_args;
//     char *value;
//     char *new_arg;
//     char *temp;
//     int i, j, k;

//     while (current != NULL)
//     {
//         current_command = current->command;
//         while (current_command != NULL)
//         {
//             args = current_command->args;
//             i = 0;
//             while (args[i] != NULL)
//             {
//                 new_arg = ft_strdup("");
//                 j = 0;
//                 while (args[i][j] != '\0')
//                 {
//                     if (args[i][j] == '$')
//                     {
//                         k = j;
//                         while (args[i][k] && (isalpha(args[i][k + 1]) || isdigit(args[i][k + 1]) || args[i][k + 1] == '_'))
//                         {
//                             k++;
//                         }
//                         // Get the value of the environment variable
//                         value = check_value_env(ft_substr(args[i], j, k - j + 1), env_list);
                    
//                         temp = new_arg;
//                         new_arg = ft_strjoin(temp, value);
//                         free(temp);

//                         // Skip over the variable name in the original string
//                         while (args[i][j] && (isalpha(args[i][j + 1]) || isdigit(args[i][j + 1]) || args[i][j + 1] == '_'))
//                         {
//                             j++;
//                         }
//                     }
//                     else
//                     {
//                         // Append the current character if it's not part of an environment variable
//                         temp = new_arg;
//                         new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));
//                         free(temp);
//                     }
//                     j++;
//                 }
//                 // free(args[i]);
//                 if(args[i][0] == '"')
//                 {
//                 split_args = ft_split3(new_arg, ' ');
//                 free(new_arg);
//                 // args = resize_args(args, ft_strlen1(split_args) + ft_strlen1(args));
//                 int c = 0;
//                 k = i;
//                 while (split_args[c] != NULL)
//                 {
//                     args[i] = split_args[c];
//                     i++;
//                     c++;
//                 }
//                 args[i] = NULL;
//                 printf("--------------------\n");
//                 }
//                 else
//                 args[i++] = new_arg; // Replace with the expanded string
//             }
//             current_command = current_command->next;
//         }
//         current = current->next;
//     }
    
// }


char **resize_args(char **args, int new_size)
{
    char **new_args = (char **)malloc(sizeof(char *) * (new_size + 1));
    int i = 0;

    if (new_args == NULL)
    {
        perror("Memory allocation failed\n");
        exit(1);
    }

    while (args[i] != NULL)
    {
        new_args[i] = args[i];
        i++;
    }

    new_args[i] = NULL;
    free(args);
    return new_args;
}

void expansion_process(t_node **head, t_env *env_list)
{
    t_node *current = *head;
    t_command *current_command;
    char **args;
    char **split_args;
    char *value;
    char *new_arg;
    char *temp;
    int i, j, k, num_splits, original_size;

    while (current != NULL)
    {
        current_command = current->command;
        while (current_command != NULL)
        {
            args = current_command->args;
            i = 0;
            while (args[i] != NULL)
            {
                new_arg = ft_strdup(""); // Start with an empty string
                j = 0;
                while (args[i][j] != '\0')
                {
                    if (args[i][j] == '$' && args[i][j - 1] != '\'')
                    {
                        k = j;
                        while (args[i][k] && (isalpha(args[i][k + 1]) || isdigit(args[i][k + 1]) || args[i][k + 1] == '_'))
                            k++;
                        // Get the value of the environment variable
                        value = check_value_env(ft_substr(args[i], j, k - j + 1), env_list);
                        temp = new_arg;
                        new_arg = ft_strjoin(temp, value);
                        free(temp);
                        // Skip over the variable name in the original string
                        while (args[i][j] && (isalpha(args[i][j + 1]) || isdigit(args[i][j + 1]) || args[i][j + 1] == '_'))
                            j++;
                    }
                    else
                    {
                        // Append the current character if it's not part of an environment variable
                        temp = new_arg;
                        new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));
                        free(temp);
                    }
                    j++;
                }

                // Handle case where args[i] contains quotes and needs to be split
                if (args[i][0] == '"')
                {
                    args[i++] = new_arg; // Replace with the expanded string
                }
                else
                {
                    split_args = ft_split3(new_arg, ' ');
                    free(new_arg);

                    // Calculate the size of the args array
                    original_size = 0;
                    while (args[original_size] != NULL)
                        original_size++;

                    // Calculate the number of splits
                    num_splits = 0;
                    while (split_args[num_splits] != NULL)
                        num_splits++;

                    // Resize the args array to accommodate new arguments
                    args = resize_args(args, original_size + num_splits - 1);

                    // Insert the split arguments into the args array
                    k = original_size + num_splits - 1;
                    while (k >= i + num_splits)
                    {
                        args[k] = args[k - num_splits + 1];
                        k--;
                    }

                    k = 0;
                    while (split_args[k] != NULL)
                    {
                        args[i + k] = split_args[k];
                        k++;
                    }
                    free(split_args);
                    i += k;
                }
            }
            current_command->args = args;
            current_command = current_command->next;
        }
        current = current->next;
    }
}
