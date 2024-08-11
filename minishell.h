#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

#ifndef MINISHELL_H
#define MINISHELL_H

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 100

typedef enum 
{
    TOKEN_COMMAND,
    TOKEN_ARG,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_FILE,
    TOKEN_SEMICOLON,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_EOF,
    HEREDOC
} TokenType;

typedef struct 
{
    char value[MAX_TOKEN_LENGTH];
    TokenType type;
    int token_count;
} TokenInfo;

typedef struct TokenNode 
{
    TokenInfo info;
    struct TokenNode *next;
} TokenNode;

TokenInfo *tokenizer(char **inputs);
TokenType get_token_type(char *c);
int is_special_char(char c);
int is_space(char c);
TokenNode *ArrayIntoNodes(TokenInfo *tokens);
TokenNode *create_node(TokenInfo token);
void print_linked_list(TokenNode *head);
void free_linked_list(TokenNode *head);
int check_syntax(TokenNode *head);
int check_special_chars(TokenNode *head);
void add_spaces(char *input);
int check_quotes(TokenNode *head, char c);
int check_quotes_spiclal_chars(TokenNode *head, char c);
char **split_by_quots(char *input, char c);
#endif

