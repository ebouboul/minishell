
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#ifndef MINISHELL_H
#define MINISHELL_H

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 100

typedef struct MemoryNode
{
	void				*ptr;
	struct MemoryNode	*next;
}						MemoryNode;

typedef struct s_memory_context 
{
    MemoryNode *head;
} MemoryContext;

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
int ft_strcmp(const char *s1, const char *s2);

int check_special_chars(TokenNode *head);
char *add_spaces(char *input);
int check_quotes(TokenNode *head, char c);
int check_quotes_spiclal_chars(TokenNode *head, char c);
char **split_by_quots(char *input);
int track_quots(char *input);
int print_error(char *str);

void fill_env_list(char **env, t_env *env_list);
void print_env_list(t_env *env_list);
int check_key_if_deja(t_env *env_list, char *var);
int check_special_validity(TokenNode *head);
int check_syntax_double_special_charcters(TokenNode *head);

int check_syntax_special_Face_to_Face(TokenNode *head);
char *check_value_env(char *str, t_env *head);
// void expansion_process(TokenNode *head, t_env *key);
void replace_quotes_by_spaces(char *input);
void replace_quotes_by_spaces_and_join(char *input, int closed);
void remove_quotes(char *input, int closed);
void remove_quotes_from_first_and_last(char *input);
int execute_builtin(t_node *head, t_env **env_list);
char *ft_strndup(char *s, int n);
void add_env_node(t_env **current, char *key, char *value);
t_node *convert_to_node_list(TokenNode *token_list);
void print_node_list(t_node *node_list);
void remove_quotes_and_join(t_node *head);
// void unset_env1(char **env, char *input);
char **get_key_value(char *var);
int check_key_from_env(t_env *env_list, char *key);
char **get_key_value(char *var);
void expansion_process(t_node **head, t_env *key, int exit_status);
void remove_all_quotes_and_join(char *input);
int ft_strlen1(char **str);
int is_numeric(const char *str);
char *remove_all_quotes2(const char *str);
void remove_quotes_from_first_and_last_only(char *input);

// builtins functions:
int is_builtin(char *command);
int ft_export(t_command *command, t_env **env_list);
char *get_env_value(t_env *env_list, char *key);
int	ft_unset(t_command *command, t_env **env_list);

// execution functions v1:
// void execute_cmds(t_node *head, t_env **env_list, int *exit_status);
// void execute_single_command(t_node *node, t_env **env_list, int *exit_status);
// char *find_executable(const char *command, char **paths);
// char **create_env_array(t_env *env_list);
// char **split_path(const char *path);
void replace_wildcard_in_args(t_node *head);
char	**resize_args(char **args, int new_size);
void	shift_args(char **args, int i, int original_size, int num_splits);
int	is_heredoc(t_node *node);
void	execute_heredoc(t_node *current, t_env **env_list, int *exit_status);
void	execute_piped_commands(t_node *current, t_env **env_list, int *exit_status);
void	execute_cmds(t_node *head, t_env **env_list, int *exit_status);
int	is_redirection(t_node *node);
void expand_redirect(t_redirect **redirect, t_env *env_list, int exit_status);
// execution functions v2:
// char *gett_env_value(const char *key, t_env *env_list);
// int execute_external(t_command *command, t_env *env_list);
// void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list, int *exit_status);
// void handle_heredoc_token(t_node *head, t_env **env_list);
// int if_file_has_permission(char *file);
// char *find_executable_in_path(char *command, t_env *env_list);
// int check_file_permissions(char *file);
// int execute_command(char *executable_path, char **args, t_env *env_list);
// int is_heredoc(t_node *node);
char	*gett_env_value(const char *key, t_env *env_list);
char	*find_executable_in_path(char *command, t_env *env_list);
int	check_file_permissions(char *file);
int	execute_command(char *executable_path, char **args, t_env *env_list);
int	execute_external(t_command *command, t_env *env_list);
void expan_herdoc(char **args, t_env *env_list, int exit_status);

// execution_v_1_1 functions
void	handle_builtin_command(t_node *node, t_env **env_list, int *exit_status);
void	execute_external_command(t_node *node, t_env **env_list, int *exit_status);
void	execute_single_command(t_node *node, t_env **env_list, int *exit_status);
char	*find_executable(const char *command, char **paths);
int	count_env_variables(t_env *env_list);

// execution_v1_2 functions
char	*create_env_string(t_env *env);
char	**create_env_array(t_env *env_list);
int	count_path_components(const char *path);
char	**split_path(const char *path);

// signals functions
void handler(int signum);
void handler_c(int signum);

// heredocP1 functions
char* create_temp_filename(void);
int open_temp_file(const char *filename, int flags);
void write_to_file(int fd, const char *str);
char **process_heredoc_input(const char *delimiter);
void	handle_single_heredoc(t_redirect *redirect, const char *temp_file, t_env **env_list, int *exit_status);

// heredocP2 functions
void execute_command_with_heredoc(t_node *temp, t_env **env_list, int *exit_status, const char *temp_file);
void handle_heredoc(t_node *node, t_env **env_list, int *exit_status);

// redirectionsP1 functions
void handle_open_error(void);
void handle_dup2_error(void);
int open_file(const char *str, int flags);
void redirect_output(const char *str, int flags);
void redirect_input(const char *str);

// redirectionsP2 functions
void handle_single_redirection(t_redirect *redirect);
void handle_redirections(t_node *node, t_env **env_list, int *exit_status);
void wait_for_children(pid_t last_pid);

//execute_pipe_and_cmds functions

void setup_pipe(int fd[2]);
void handle_child_io(int prev_pipe, int fd[2], t_node *current);
void handle_child_process(int prev_pipe, int fd[2], t_node *current, t_env **env_list, int *exit_status);
void handle_parent_io(int *prev_pipe, int fd[2], t_node *current, pid_t *last_pid, pid_t pid);
void handle_pipe_and_multiple_commands(t_node *head, t_env **env_list, int *exit_status);




// free functions
void *gc_malloc(size_t size);
void gc_free_all();
void gc_free(void *ptr);


#endif

