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
    TOKEN_EOF,
    TOKEN_HEREDOC
} TokenType;

typedef struct 
{
    char *value;
    TokenType type;
    int token_count;
} TokenInfo;

typedef struct TokenNode 
{
    TokenInfo info;
    struct TokenNode *next;
} TokenNode;

typedef struct 
{
    char *key;
    char *value;
} env;

typedef struct t_env
{
    env *env;
    // int *index;
   struct t_env *next;
} t_env;


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
char **split_by_quots(char *input);
int truck_quots(char *input, char c);
void unset_env(char **env, char *input);
// void export_env(char **env, char *var);
void fill_env_list(char **env, t_env *env_list);
void print_env_list(t_env *env_list);
int check_key_if_deja(t_env *env_list, char *var);
// int check_RRAH(TokenNode *head);
int check_special_validity(TokenNode *head);
int check_syntax_special_Face_to_Face(TokenNode *head);
char *check_value_env(char *str, t_env *head);
void expansion_process(TokenNode *head, t_env *key);
void replace_quotes_by_spaces(char *input);
void replace_quotes_by_spaces_and_join(char *input, int closed);
void remove_quotes(char *input, int closed);

#endif

