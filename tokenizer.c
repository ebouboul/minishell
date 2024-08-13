#include "minishell.h"


int is_space(char c) 
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

int is_special_char(char c) 
{
    return c == '|' || c == '<' || c == '>' || c == '&' || c == ';';
}

int ft_strcmp(char *s1, char *s2)
{
    int i;
    i = 0;

    while(s1[i] && s2[i] && s1[i] == s2[i])
    i++;
    return(s2[i] - s1[i]);
}


TokenType get_token_type(char *c) 
{
    if (ft_strcmp(c, "|") == 0)
        return TOKEN_PIPE;
    else if (ft_strcmp(c, ">>") == 0)
        return TOKEN_APPEND; 
    else if (ft_strcmp(c, "<") == 0)
        return TOKEN_REDIRECT_IN;
    else if (ft_strcmp(c, ">") == 0)
        return TOKEN_REDIRECT_OUT;
    else if (ft_strcmp(c, ";") == 0)
        return TOKEN_SEMICOLON;
    else if (ft_strcmp(c, "<<") == 0)
        return TOKEN_HEREDOC;

    else
        return TOKEN_COMMAND;
}

TokenInfo *tokenizer(char **inputs)
{
    int (i),(j);
    int token_count;

    i = 0; 
    j = 0;
    token_count = 0;
    TokenInfo *tokens = malloc((100) * sizeof(TokenInfo)); 

    if (!tokens) 
    {
        perror("Memory allocation failed\n");
        return NULL;
    }
    
    while (inputs[j])
    {
        i = 0;
        int k = 0;
        tokens[token_count].value = malloc(ft_strlen(inputs[j]) + 1);
                while (inputs[j][i] != '\0' && token_count < MAX_TOKENS )
                    {
                        while (is_space(inputs[j][i])) 
                            i++;
                        if (inputs[j][i] == '\0') 
                            break;
                        if (is_special_char(inputs[j][i])) 
                        {
                            tokens[token_count].type = get_token_type(inputs[j]);
                            while(inputs[j][k])
                            {
                            tokens[token_count].value[k++] = inputs[j][i++];
                            tokens[token_count].value[k] = '\0';
                            }
                            token_count++;
                        }
                        else if(token_count > 0 &&(tokens[token_count - 1].type == TOKEN_REDIRECT_IN || tokens[token_count - 1].type == TOKEN_REDIRECT_OUT || 
                        tokens[token_count - 1].type == TOKEN_APPEND || tokens[token_count - 1].type == TOKEN_HEREDOC))
                        {
                            tokens[token_count].type = TOKEN_FILE;
                            while(inputs[j][k])
                            {
                            tokens[token_count].value[k++] = inputs[j][i++];
                            tokens[token_count].value[k] = '\0';
                            }
                            token_count++;
                        }
                        else if(token_count > 0 &&(tokens[token_count - 1].type == TOKEN_FILE))
                        {
                            tokens[token_count].type = TOKEN_ARG;
                            while(inputs[j][k])
                            {
                            tokens[token_count].value[k++] = inputs[j][i++];
                            tokens[token_count].value[k] = '\0';
                            }
                            token_count++;
                        }
                        else if(token_count > 0 &&(tokens[token_count - 1].type == TOKEN_COMMAND))
                        {
                            tokens[token_count].type = TOKEN_ARG;
                            while(inputs[j][k])
                            {
                            tokens[token_count].value[k++] = inputs[j][i++];
                            tokens[token_count].value[k] = '\0';
                            }
                            token_count++;
                        }
                        else
                        {
                            int o = 0;
                            tokens[token_count].type = TOKEN_COMMAND;
                            while (inputs[j][i] != '\0' && !is_space(inputs[j][i]) && !is_special_char(inputs[j][i]) && o < MAX_TOKEN_LENGTH - 1) 
                                tokens[token_count].value[o++] = inputs[j][i++];
                            tokens[token_count].value[o] = '\0';
                            token_count++;
                        }
                    }
        j++;
    }
    
    tokens[token_count].value = malloc(1);

    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';

    return tokens;
}
