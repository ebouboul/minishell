#include "../minishell.h"


int is_special_char2(char c) 
{
    // Include all special characters you want to check
    if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')' || c == '<' || c == '>')
        return 1;
    return 0;
}

char *add_spaces(char *input, MemoryManager *manger)
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
    char *new_input = (char *)gc_malloc(manger, length + (j * 2) + 1);
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
    new_input[j] = '\0';
    return new_input;
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
void remove_quotes_from_first_and_last_only(char *input)
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
    while (input[i] != '\0' && flag != 0)
    {
        input[i] = input[i + 1];
        i++;
    }
    remove_quotes_after_equal(input);
}



