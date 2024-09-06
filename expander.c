

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

char *get_env_value3(t_env *env_list, char *key)
{
    t_env *current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->env->key, key) == 0)
            return current->env->value;
        current = current->next;
    }
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
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$')
            return i;
        i++;
    }
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
int	in_quotes(char *str, int i, char c)
{
	int	flag;

	flag = 0;
	while (i >= 0)
	{
		if (str[i] == '\"')
		{
			i--;
			while (i >= 0 && str[i] != '\"')
			{
				if (str[i] == c)
					flag = 1;
				i--;
			}
			if (i < 0 && flag == 1)
				return (0);
			else
				i--;
			flag = 0;
		}
		else
			i--;
	}
	return (1);
}
int	dstrchr2(char *s, char c, int *i)
{
	char	quote;

	(*i)++;
	quote = s[*i - 1];
	while (s[*i] && s[*i] != quote)
	{
		if (s[*i] == c && s[*i + 1] != '\0' && quote == '\"')
			return (1);
		(*i)++;
	}
	if (s[*i] == quote)
		(*i)++;
	return (0);
}
char	*dstrchr(char *s, char c, int *flag)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if ((s[i] == '\"' && ft_strchr(&s[i + 1], '\"')) || s[i] == '\''
			|| !in_quotes(s, (ft_strlen(s) - 1), c))
		{
			if (s[i] != '\"' && s[i] != '\'')
			{
				while (s[i] && s[i] != c && s[i] != '\"')
					i++;
				if (s[i] == c)
					return (*flag = 1, &s[i]);
			}
			else if (dstrchr2(s, c, &i))
				return (*flag = 1, &s[i]);
		}
		else if (s[i] == c)
			return (*flag = i, &s[i]);
		else
			i++;
	}
	return (NULL);
}
int check_if_dollar_in_single_quotes(char *str)
{
    int i = 0;
    int flag = 0;
    while (str[i] != '\0')
    {
        if(str[i] == '\'' && str[i + 1] == '"' && flag == 0)
            flag = 1;
        if (str[i] == '\'' && flag == 0)
            flag = 1;
        if (str[i] == '"' && str[i + 1] == '\'' && flag == 0)
            flag = 2;
        if (str[i] == '$' && flag == 1)
            return 1;
        i++;
    }
    return 0;
}
// void handle_expansion(char **args, int j, int i, t_env *env_list, int exit_status)
// {
//     char *new_arg = ft_strdup("");
//     char *temp, *value;
//     int  k;
//     int in_single_quotes = 0;  // Flag to track single quotes
//     int in_double_quotes = 0;  // Flag to track double quotes
//     j = 0;

//     while (args[i][j] != '\0')
//     {
//         // Toggle single-quote flag
//         if (args[i][j] == '\'' && !in_double_quotes)
//         {
//             in_single_quotes = !in_single_quotes;
//             temp = new_arg;
//             new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));  // Append the single quote itself
//             gc_free(temp);
//             j++;
//             continue;
//         }

//         // Toggle double-quote flag
//         if (args[i][j] == '"' && !in_single_quotes)
//         {
//             in_double_quotes = !in_double_quotes;
//             temp = new_arg;
//             new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));  // Append the double quote itself
//             gc_free(temp);
//             j++;
//             continue;
//         }

//         // Handle expansion only if not inside single quotes
//         if (!in_single_quotes && args[i][j] == '$' && args[i][j + 1] != '\0' 
//             && (isalpha(args[i][j + 1]) || args[i][j + 1] == '_' || args[i][j + 1] == '?' || args[i][j + 1] == '$'))
//         {
//             if (args[i][j + 1] == '?')
//             {
//                 value = ft_itoa(exit_status);
//                 temp = new_arg;
//                 new_arg = ft_strjoin(temp, value);
//                 gc_free(temp);
//                 j += 2;
//                 continue;
//             }
//             k = j;
//             while (args[i][k] && (isalpha(args[i][k + 1]) || isdigit(args[i][k + 1]) || args[i][k + 1] == '_'))
//                 k++;
//             value = check_value_env(ft_substr(args[i], j, k - j + 1), env_list);
//             temp = new_arg;
//             new_arg = ft_strjoin(temp, value);
//             gc_free(temp);
//             j = k + 1;
//         }
//         else
//         {
//             // Append the current character
//             temp = new_arg;
//             new_arg = ft_strjoin(temp, ft_substr(args[i], j, 1));
//             gc_free(temp);
//             j++;
//         }
//     }

//     if (new_arg[0] != '\0')
//     {
//         args[i] = new_arg;
//     }
//     else
//     {
//         remove_args(args, i);
//     }
// }


// void handle_splitting(char ***args, int i)
// {
//     char **split_args = ft_split3((*args)[i], ' ');
//     int original_size = 0, num_splits = 0, k;

//     while ((*args)[original_size] != NULL)
//         original_size++;
//     while (split_args[num_splits] != NULL)
//         num_splits++;

//     *args = resize_args(*args, original_size + num_splits - 1);

//     k = original_size + num_splits - 1;
//     while (k >= i + num_splits)
//     {
//         (*args)[k] = (*args)[k - num_splits + 1];
//         k--;
//     }
//     k = 0;
//     while (k < num_splits)
//     {
//         (*args)[i + k] = split_args[k];
//         remove_all_quotes_and_join((*args)[i + k]);
//         k++;
//     }  gc_free(split_args);
// }

// void process_arguments(t_command *current_command, t_env *env_list, int exit_status)
// {
//     char **args = current_command->args;
//     int i = 0;
//     int j = 0;
//     int k = 0;

//     while (args[i] != NULL )
//     {
//         if (dstrchr(args[i], '$', &j))
//         {
//             handle_expansion(args, j, i, env_list, exit_status);
//             if (args[i] && args[i][0] != '"' && ft_strchr(args[i], ' ') != NULL && ft_strncmp(args[0], "export", 6) != 0)
//                 handle_splitting(&args, i);
//         }
//         if (ft_strchr(args[i], '$') == NULL || args[i][dollar_position(args[i]) + 1] == '\0' 
//             || dollar_position(args[i]) == 0 || !dstrchr(args[i], '$', &k)) 
//         i++;
//     }
//     current_command->args = args;
// }

void append_char(char c, char **new_arg)
{
    char temp[2] = {c, '\0'};
    char *old_arg = *new_arg;
    *new_arg = ft_strjoin(old_arg, temp);
    gc_free(old_arg);
}

void handle_single_quote(char *arg, char **new_arg, int *in_single_quotes, int *j)
{
    *in_single_quotes = !(*in_single_quotes);
    append_char(arg[(*j)++], new_arg);
}

void handle_double_quote(char *arg, char **new_arg, int *in_double_quotes, int *j)
{
    *in_double_quotes = !(*in_double_quotes);
    append_char(arg[(*j)++], new_arg);
}


// void expand_variable(char *arg, char **new_arg, int *j, t_env *env_list)
// {
//     int k = *j;
//     char *var_name, *value, *temp;
//     value = NULL;

//     while (arg[k] && (isalpha(arg[k + 1]) || isdigit(arg[k + 1]) || arg[k + 1] == '_'))
//         k++;
//     printf("%s\n", arg);
//     var_name = ft_substr(arg, *j + 1, k - *j);
//     if (isdigit(var_name[0]))
//         value = ft_strdup(var_name + 1);
//     else
//         value = get_env_value3(env_list, var_name);
//     gc_free(var_name); // Free the temporary variable name string

//     // Append the expanded value to the new argument
//     temp = *new_arg;
//     *new_arg = ft_strjoin(temp, value);
//     gc_free(temp);

//     *j = k + 1; // Move past the expanded variable

// }

char *ft_charjoin(char *s, char c)
{
    char *result;
    int i;
    int len;

    len = ft_strlen(s);
    result = (char *)gc_malloc(len + 2);
    if (result == NULL)
    {
        perror("Memory allocation failed\n");
        return NULL;
    }

    i = 0;
    while (s[i] != '\0')
    {
        result[i] = s[i];
        i++;
    }

    result[i] = c;
    result[i + 1] = '\0';
    return result;
}
int is_quot1e(char c)
{
    return (c == '\'' || c == '"');
}

void expand_variable(char *arg, char **new_arg, int *j, t_env *env_list)
{
    int k = *j;
    char *var_name, *value, *temp;
    value = NULL;

    // Iterate to find the end of the variable name
    while (arg[k] && (isalpha(arg[k + 1]) || isdigit(arg[k + 1]) || arg[k + 1] == '_'))
        k++;

    // Extract the variable name
    var_name = ft_substr(arg, *j + 1, k - *j);
    if (var_name == NULL) {
        perror("ft_substr");
        exit(EXIT_FAILURE);
    }

    // Handle numeric variable cases (e.g., $1)
    if (isdigit(var_name[0])) {
        value = ft_strdup(var_name + 1);
    } else {
        value = get_env_value3(env_list, var_name);
    }

    gc_free(var_name); // Free the temporary variable name string

    if (value == NULL) {
        perror("get_env_value3");
        exit(EXIT_FAILURE);
    }

    // Append the expanded value to the new argument
    temp = *new_arg;
    *new_arg = ft_strjoin(temp, value);
    if (*new_arg == NULL) {
        perror("ft_strjoin");
        exit(EXIT_FAILURE);
    }

    gc_free(temp);
    // Move past the expanded variable
    *j = k + 1;

    // Handle the case for trailing characters after expansion
    while (arg[*j] != '\0') {
        if (arg[*j] == '$') {
            // If another '$' character is found, expand it
            expand_variable(arg, new_arg, j, env_list);
        } else {
            // Append any other character
            append_char(arg[*j], new_arg);
            (*j)++;
        }
    }
}

// void expand_variable(char *arg, char **new_arg, int *j, t_env *env_list)
// {
//     int k = *j;
//     char *var_name, *value, *temp;
//     value = NULL;

//     // Iterate to find the end of the variable name
//     while (arg[k] && (isalpha(arg[k + 1]) || isdigit(arg[k + 1]) || arg[k + 1] == '_' || !is_quot1e(arg[k + 1])))
//         k++;

//     // Extract the variable name
//     var_name = ft_substr(arg, *j + 1, k - *j);
//     // printf("%s\n", var_name);
//     if (var_name == NULL) {
//         perror("ft_substr");
//         exit(EXIT_FAILURE);
//     }

//     // Handle numeric variable cases (e.g., $1)
//     if (isdigit(var_name[0])) {
//         value = ft_strdup(var_name + 1);
//     } else {
//         value = get_env_value3(env_list, var_name);
//     }

//     gc_free(var_name); // Free the temporary variable name string

//     if (value == NULL) {
//         perror("get_env_value3");
//         exit(EXIT_FAILURE);
//     }

//     // Append the expanded value to the new argument
//     temp = *new_arg;
//     *new_arg = ft_strjoin(temp, value);
//     if (*new_arg == NULL) {
//         perror("ft_strjoin");
//         exit(EXIT_FAILURE);
//     }

//     gc_free(temp);
//     // Move past the expanded variable
//     *j = k + 1;

//     // If there's another '$' character, continue expanding
//     if ((int)ft_strlen(arg) >= *j && arg[*j] == '$' && arg[*j + 1] != '\0')
//         expand_variable(arg, new_arg, j, env_list);
//     else if (arg[*j] != '\0')
//         append_char(arg[*j], new_arg);
//     (*j)++;
// }


void handle_dollar_sign(char *arg, char **new_arg, int *j, t_env *env_list, int exit_status)
{
    char *temp, *value;
    if (arg[*j + 1] == '?')
    {
        value = ft_itoa(exit_status);
        temp = *new_arg;
        *new_arg = ft_strjoin(temp, value);
        gc_free(temp);
        *j += 2;
    }
    else
        expand_variable(arg, new_arg, j, env_list);
}






void update_args(char **args, char *new_arg, int i)
{
    if (new_arg[0] != '\0')
        args[i] = new_arg;
    else
        remove_args(args, i);
}

int get_arg_size(char **args)
{
    int size = 0;
    while (args[size] != NULL)
        size++;
    return size;
}

void shift_args(char **args, int i, int original_size, int num_splits)
{
    int k = original_size + num_splits - 1;
    while (k >= i + num_splits)
    {
        args[k] = args[k - num_splits + 1];
        k--;
    }
}

void split_and_remove_quotes(char ***args, char **split_args, int i, int num_splits)
{
    for (int k = 0; k < num_splits; k++)
    {
        (*args)[i + k] = split_args[k];
        remove_all_quotes_and_join((*args)[i + k]);
    }
}


void handle_splitting(char ***args, int i)
{
    char **split_args = ft_split3((*args)[i], ' ');
    int original_size = get_arg_size(*args);
    int num_splits = get_arg_size(split_args);
    *args = resize_args(*args, original_size + num_splits - 1);

    shift_args(*args, i, original_size, num_splits);
    split_and_remove_quotes(args, split_args, i, num_splits);
    gc_free(split_args);
}




int no_expansion_needed(char *arg)
{
    int k;
    return ft_strchr(arg, '$') == NULL || arg[dollar_position(arg) + 1] == '\0' 
        || dollar_position(arg) == 0 || !dstrchr(arg, '$', &k);
}
void handle_expansion(char **args, int i, t_env *env_list, int exit_status)
{
    char *new_arg = ft_strdup("");
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    int j = 0;

    while (args[i][j] != '\0')
    {
  
        if (args[i][j] == '\'' && !in_double_quotes)
            handle_single_quote(args[i], &new_arg, &in_single_quotes, &j);
        else if (args[i][j] == '"' && !in_single_quotes)
            handle_double_quote(args[i], &new_arg, &in_double_quotes, &j);
        else if (!in_single_quotes && args[i][j] == '$')
            handle_dollar_sign(args[i], &new_arg, &j, env_list, exit_status);
        
        if (j >= (int)ft_strlen(args[i]))
            break;
    }

    update_args(args, new_arg, i);
}
int is_dollar_only(char *str)
{
    if (str[0] == '$' && str[1] == '\0')
        return 1;
    return 0;
}


void process_arguments(t_command *current_command, t_env *env_list, int exit_status)
{
    char **args = current_command->args;
    int i = 0, j = 0;

    while (args[i] != NULL)
    {
        if (dstrchr(args[i], '$', &j) && is_dollar_only(args[i]) == 0)
        {
            handle_expansion(args, i, env_list, exit_status);
            if (args[i] && args[i][0] != '"' && ft_strchr(args[i], ' ') != NULL 
                && ft_strncmp(args[0], "export", 6) != 0)
                handle_splitting(&args, i);
        }
        if (no_expansion_needed(args[i]))
            i++;
    }
    current_command->args = args;
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

