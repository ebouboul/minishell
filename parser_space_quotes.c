#include "minishell.h"

int is_here_doc_red(char *input)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if ((input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<' )||(input[i] == '>' && input[i + 1] == '>'  ))
            return 1;
        i++;
    }
    return 0;
}
// int is_special_char2(char c) 
// {
//     if(c == '|' || c == '&' || c == ';' || c == '(' || c == ')')
//         return 1;
//     return 0;
// }
// char *add_spaces(char *input)
// {
//     int i = 0;
//     int j = 0;
//     while (input[i] != '\0')
//     {
//         if(is_special_char2(input[i]) && (i == 0 || input[i - 1] != ' ' || input[i + 1] != ' '))
//             j++;
//         i++;
//     }
//     char *new_input = (char*)gc_malloc(ft_strlen(input) + (j + 1) * 2 + 1);
//     i = 0;
//     j = 0;
//     while (input[i] != '\0') 
//     {
//         if (is_special_char2(input[i]) && (i == 0 || input[i - 1] != ' ' || input[i + 1] != ' ')) 
//         {
//             {
//                 new_input[j++] = ' ';
//                 new_input[j++] = input[i++];
//                 new_input[j++] = ' ';
//             } 
//          }
//          else 
//                 new_input[j++] = input[i++];
//     }
//     new_input[j] = '\0';
//     return new_input;
// }

int is_special_char2(char c) 
{
    // Include all special characters you want to check
    if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<' || c == '>')
        return 1;
    return 0;
}

char *add_spaces(char *input)
{
    int i = 0;
    int j = 0;
    int length = ft_strlen(input);
    
    // Count the number of extra spaces needed
    while (input[i] != '\0')
    {
        if (is_special_char2(input[i])) 
        {
            if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))  // Special case for '>>'
            {
                if (i == 0 || input[i - 1] != ' ' || input[i + 2] != ' ')
                    j += 2;  // Need two extra spaces for '>>'
                i++;  // Skip the next '>' character since it's part of '>>'
            } 
            else if (i == 0 || input[i - 1] != ' ' || input[i + 1] != ' ')
                j++;  // Need one extra space for other special characters
        }
        i++;
    }

    // Allocate new memory for the input with additional spaces
    char *new_input = (char *)gc_malloc(length + (j * 2) + 1);
    if (!new_input)
        return NULL; // Handle memory allocation failure

    i = 0;
    j = 0;

    // Add spaces around special characters
    while (input[i] != '\0')
    {
        if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))  // Special case for '>>'
        {
            if (j == 0 || new_input[j - 1] != ' ')
                new_input[j++] = ' ';
            new_input[j++] = input[i++];
            new_input[j++] = input[i++];
            if (input[i] != ' ')
                new_input[j++] = ' ';
        }
        else if (is_special_char2(input[i])) 
        {
            if (j == 0 || new_input[j - 1] != ' ')
                new_input[j++] = ' ';
            new_input[j++] = input[i++];
            if (input[i] != ' ')
                new_input[j++] = ' ';
        }
        else 
        {
            new_input[j++] = input[i++];
        }
    }
    
    new_input[j] = '\0';  // Null-terminate the new string
    return new_input;
}

int check_quotes(TokenNode *head, char c)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_COMMAND || current->info.type == TOKEN_ARG) 
        {
            if (current->info.value[0] == c && current->info.value[strlen(current->info.value) - 1] != c)
                return(print_error("Error: quotes not closed\n"));
        }
        current = current->next;
    }
    return 0;
}

int check_quotes_spiclal_chars(TokenNode *head, char c)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_COMMAND || current->info.type == TOKEN_ARG) 
        {
            if (current->info.value[0] == c && current->next != NULL && current->next->info.type != TOKEN_COMMAND && current->next->info.type != TOKEN_ARG) 
                return(print_error("Error: special characters not allowed after quotes\n"));
        }
        current = current->next;
    }
    return 0;
}
int check_syntax_double_commands(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_COMMAND) {
            if (current->next != NULL && current->next->info.type == TOKEN_COMMAND) 
                return(print_error("Error: Double commands\n"));
        }
        current = current->next;
    }
    return 0;
}
int check_special_chars(TokenNode *head)
{
    TokenNode *current = head;
    if (current->info.type == TOKEN_PIPE || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT || current->info.type == TOKEN_APPEND ) 
            return(print_error("Error: Special character at the beginning of the command\n"));
    while (current != NULL) {
        if (current->info.type == TOKEN_PIPE || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT || current->info.type == TOKEN_APPEND || current->info.type == TOKEN_HEREDOC) 
        {
            if (current->next == NULL) 
                return(print_error("Error: Special character at the end of the command\n"));
        }
        current = current->next;
    }
    return 0;
}
int check_syntax_double_special_charcters(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_APPEND || current->info.type == TOKEN_HEREDOC || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT) 
        {
            if (current->next != NULL && current->next->info.type != TOKEN_FILE) 
                return(print_error("Error: Double special characters\n"));
        }
        current = current->next;
    }
    return 0;
}
int check_syntax_special_Face_to_Face(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_APPEND || current->info.type == TOKEN_HEREDOC || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT) 
        {
            if ((current->next != NULL && current->next->info.type == TOKEN_APPEND) || (current->next != NULL && current->next->info.type == TOKEN_HEREDOC) || (current->next != NULL && current->next->info.type == TOKEN_REDIRECT_IN)|| (current->next != NULL && current->next->info.type == TOKEN_REDIRECT_OUT)) 
                return(print_error("Error: Face to Face\n"));
        }
        else if (current->info.type == TOKEN_PIPE)
            {
                if(current->next == NULL || current->next->info.type == TOKEN_PIPE )
                    return(print_error("Error: Face to Face\n"));
            }
        current = current->next;
    }
    return 0;
}
int check_special_validity(TokenNode *head)
{
    TokenNode *current = head;
    while (current != NULL) 
    {
        if (current->info.type == TOKEN_COMMAND) 
        {
            if (strchr(current->info.value, '<') || strchr(current->info.value, '>')) 
            {
                    return(print_error("Error: special characters not allowed in command\n"));
            }
        }
        current = current->next;
    }
    return 0;
}
void replace_quotes_by_spaces_and_join(char *input, int closed)
{
    int i = 0;
    int j = 0;
    
    while (!closed && input[i] != '\0') 
    {
        if (input[i] == '"' || input[i] == '\'')
        {
            i++;
        }
        else
        {
            input[j] = input[i];
            i++;
            j++;
        }
    }

    input[j] = '\0';
}
void remove_quotes_after_equal(char *input)
{
    int i = 0;
    int j;

    while (input && input[i] != '\0') 
    {
        if (input[i] == '=' && (input[i + 1] == '"' || input[i + 1] == '\'')) 
        {
            j = i + 1; 
            while (input[j] != '\0') 
            {
                input[j] = input[j + 1];
                j++;
            }
        }
        else
        {
            i++;
        }
    }
}

void remove_quotes_from_first_and_last(char *input)
{
    int i = 0;
    int flag = 0;
    if(ft_strlen(input) == 0)
        return;
    if(input[0] == '"')
        flag = 1;
    else if(input[0] == '\'')
        flag = 2;
    if(flag == 1 && input[strlen(input) - 1] == '"')
            input[strlen(input) - 1] = '\0';
    else if(flag == 2 && input[strlen(input) - 1] == '\'')
            input[strlen(input) - 1] = '\0';
    if(input[strlen(input) - 1] == '\"')
            input[strlen(input) - 1] = '\0';
    while (input[i] != '\0' && flag != 0) 
    {
        input[i] = input[i + 1];
        i++;
    }
    remove_quotes_after_equal(input);


}
void remove_quotes(char *input, int closed)
{
     int i = 0;
     (void)closed;
    while (input[i] != '\0') 
    {
        input[i] = input[i + 1];
        if(input[i] == '"' || input[i] == '\'')
            input[i] = ' ';
        i++;
    }

}
void remove_quotes_before_after_char(char *input)
{
    int i = 0;
    while (input[i] != '\0') 
    {
        if (input[i] != '"' || input[i] != '\'') 
        {
            i++;
        }
        else 
        {
            input[i] = ' ';
            i++;
        }
        i++;
    }
}
void remove_all_quotes_and_join(char *input)
{
    int i = 0;
    int j = 0;
    while (input[i] != '\0') 
    {
        if (input[i] == '"' || input[i] == '\'') 
        {
            i++;
        }
        else 
        {
            input[j] = input[i];
            i++;
            j++;
        }
    }
    input[j] = '\0';
}



void replace_quotes_by_spaces(char *input)
{
    int i = 0;
    while (input[i] != '\0') 
    {
        if (input[i] == '"' || input[i] == '\'') 
        {
            input[i] = ' ';
        }
        i++;
    }
}

// void remove_quotes_and_join(TokenNode *head)
// {
//     TokenNode *current = head;
//     while (current != NULL) 
//     {
//         if (current->info.type == TOKEN_COMMAND || current->info.type == TOKEN_ARG)
//             if(current->info.value[0] == '"' || current->info.value[0] == '\'')
//           remove_quotes_from_first_and_last(current->info.value);

//         current = current->next;
//     }
// }

void remove_tween_quotes(char *input)
{
    int i = 0;
    int j = 0;
    while(input[i] != '\0')
    {
        if(input[i] == '"')
        {
            i++;
            while(input[i] != '\0' && input[i] != '"')
            {
                input[j] = input[i];
                i++;
                j++;
            }
            if(input[i] == '\0')
                return;
        }
        if(input[i] == '\'')
        {
            i++;
            while(input[i] != '\0' && input[i] != '\'')
            {
                input[j] = input[i];
                i++;
                j++;
            }
            if(input[i] == '\0')
                return;
        }
        input[j] = input[i];
        i++;
        j++;
    }
    input[j] = '\0';
    printf("input = %s\n", input);
}

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

// Function to remove all quotes before and after any character
char *remove_all_quotes(const char *str)
{
    if (str == NULL)
        return NULL;

    int i = 0, j = 0;
    int len = strlen(str);
    char *result = (char *)gc_malloc(len + 1); // Allocate memory for the result

    if (result == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    while (str[i] != '\0')
    {
        // If the current character is not a quote or if it is a quote but there is no character before or after it
        if (!is_quote(str[i]) || 
           (i > 0 && !is_quote(str[i - 1]) && str[i + 1] != '\0' && !is_quote(str[i + 1])))
        {
            result[j++] = str[i];
        }
        i++;
    }

    result[j] = '\0'; // Null-terminate the result

    return result;
}

void remove_quotes_and_join(t_node *head)
{
    t_node *current = head;
    while (current != NULL) 
    {
        t_command *current_command = current->command;
        while (current_command != NULL) 
        {
            char **args = current_command->args;
            int i = 0;
            while (args[i] != NULL) 
            {
                args[i] = remove_all_quotes(args[i]);
                // remove_quotes_from_first_and_last(args[i]);
                remove_quotes_after_equal(args[i]);
                i++;
            }
            current_command = current_command->next;
        }
        current = current->next;
    }
}