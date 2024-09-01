#include "minishell.h"


char *add_spaces(char *input)
{
    int i = 0;
    int j = 0;
    while (input[i] != '\0')
    {
        if(is_special_char(input[i]))
            j++;
        i++;
    }
    char *new_input = (char*)gc_malloc(ft_strlen(input) + (j + 1) * 2 + 1);
    i = 0;
    j = 0;
    while (input[i] != '\0') 
    {
        if (is_special_char(input[i]) && (i == 0 || input[i - 1] != ' ' || input[i + 1] != ' ')) 
        {
            {
                new_input[j++] = ' ';
                new_input[j++] = input[i++];
                new_input[j++] = ' ';
            } 
         }
         else 
                new_input[j++] = input[i++];
    }
    new_input[j] = '\0';
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
                    remove_quotes_from_first_and_last(args[i]);

                i++;
            }
            current_command = current_command->next;
        }
        current = current->next;
    }
}