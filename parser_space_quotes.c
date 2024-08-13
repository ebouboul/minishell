#include "minishell.h"


void add_spaces(char *input)
{
    int i = 0;
    int j = 0;
    char *new_input = (char*)malloc(strlen(input) * 2 + 1);
    if (new_input == NULL) {
        perror("Memory allocation failed\n");
        exit(1);
    }
    while (input[i] != '\0') {
        if ((input[i] == '|' || input[i] == ';' || input[i] == '&' || input[i] == '|') && (i == 0 || input[i - 1] != ' ')) 
        {
            {
                new_input[j++] = ' ';
                new_input[j++] = input[i++];
                new_input[j++] = ' ';
            } 
         }
         else 
         {
                new_input[j++] = input[i++];
            }
    }
    new_input[j] = '\0';

    strcpy(input, new_input);
    free(new_input);

}

int check_quotes(TokenNode *head, char c)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->info.type == TOKEN_COMMAND || current->info.type == TOKEN_ARG) 
        {
            if (current->info.value[0] == c && current->info.value[strlen(current->info.value) - 1] != c)
            {
                printf("Syntax Error: Double quotes not closed\n");
                return 1;
            }
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
            {
                printf("Syntax Error: Double quotes must not contain special characters\n");
                return 1;
            }
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
            if (current->next != NULL && current->next->info.type == TOKEN_COMMAND) {
                printf("Syntax Error: Two commands in a row\n");
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}
int check_special_chars(TokenNode *head)
{
    TokenNode *current = head;
    if (current->info.type == TOKEN_PIPE || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT || current->info.type == TOKEN_APPEND || current->info.type == TOKEN_SEMICOLON ) 
    {
        printf("Syntax Error: Special character at the beginning of the command\n");
        return 1;
    }
    while (current != NULL) {
        if (current->info.type == TOKEN_PIPE || current->info.type == TOKEN_REDIRECT_IN || current->info.type == TOKEN_REDIRECT_OUT || current->info.type == TOKEN_APPEND || current->info.type == TOKEN_SEMICOLON || current->info.type == TOKEN_HEREDOC) 
        {
            if (current->next == NULL) {
                printf("Syntax Error: Special character at the end of the command\n");
                return 1;
            }
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
            if (current->next != NULL && current->next->info.type == TOKEN_FILE) 
            {
                printf("Syntax Error: Two specials in a row\n");
                return 1;
            }
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
            {
                printf("Syntax Error: Face to Face\n");
                return 1;
            }
        }
        else if (current->info.type == TOKEN_PIPE)
            {
                if(current->next->info.type == TOKEN_PIPE)
                {
                    printf("Syntax Error: Pipe to Pipe\n");
                    return 1;
                }
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
                printf("Syntax Error: Two specials in a row\n");
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}
