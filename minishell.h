/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:20:19 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/21 20:27:21 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct t_redirect
{
    char  *str;
    int flag;
    struct t_redirect *next;
} t_redirect;

typedef struct t_command
{
    t_redirect *redirect;
    char **args;
    struct t_command *next;
} t_command;

typedef struct t_node
{
    t_command *command;
    int exit_status;
    struct t_node *next;
} t_node;


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
// void unset_env(char **env, char *input);
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
int execute_builtin(TokenNode *head, t_env **env_list);
char *ft_strndup(char *s, int n);
void add_env_node(t_env **current, char *key, char *value);
t_node *convert_to_node_list(TokenNode *token_list);
void print_node_list(t_node *node_list);
void remove_quotes_and_join(TokenNode *head);
// void unset_env1(char **env, char *input);
char **get_key_value(char *var);
int check_key_from_env(t_env *env_list, char *key);
char **get_key_value(char *var);


#endif

