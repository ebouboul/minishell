/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 03:43:09 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/21 02:25:26 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <fnmatch.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define MAX_TOKEN_LENGTH 100
# define MAX_TOKENS 100

typedef struct MemoryNode
{
	void				*ptr;
	struct MemoryNode	*next;
}						t_MemoryNode;
typedef struct s_MemoryManager
{
	t_MemoryNode		*head;
}						t_MemoryManager;

typedef enum t_TokenType
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
}						t_TokenType;

typedef struct t_TokenInfo
{
	char				*value;
	t_TokenType			type;
	int					c;
}						t_TokenInfo;

typedef struct t_TokenNode
{
	t_TokenInfo			info;
	struct t_TokenNode	*next;
}						t_TokenNode;

typedef struct t_envv
{
	char				*key;
	char				*value;
}						t_envv;

typedef struct t_env
{
	t_envv				*env;
	struct t_env		*next;
}						t_env;

typedef struct t_redirect
{
	char				*str;
	char				*old_str;
	int					flag;
	struct t_redirect	*next;
}						t_redirect;

typedef struct t_command
{
	t_redirect			*redirect;
	char				**args;
	struct t_command	*next;
}						t_command;

typedef struct t_node
{
	t_command			*command;
	int					exit_status;
	struct t_node		*next;
}						t_node;

typedef struct s_token_data
{
	t_TokenInfo			*tokens;
	int					c;
	char				*input;
	int					i;
	t_MemoryManager		*manager;
}						t_TokenData;

typedef struct s_process_data
{
	int					prev_pipe;
	int					fd[2];
	pid_t				last_pid;
	pid_t				pid;
}						t_process_data;

typedef struct s_exec_context
{
	t_env				**env_list;
	int					*exit_status;
	t_MemoryManager		*gc;
	int					*k;
}						t_exec_context;

typedef struct s_var
{
	int					i;
	int					j;
	int					k;
	int					flag;
}						t_var;

typedef struct s_wildcard_vars
{
	int					pindex;
	int					sindex;
	int					backtrackpindex;
	int					backtracksindex;
}						t_wildcard_vars;

typedef struct s_replace_wildcard_vars
{
	int					match_count;
	char				*path;
	char				**matches;
	int					old_size;
	int					new_size;
}						t_replace_wildcard_vars;

// tokenizer functions
t_TokenInfo				*tokenizer(char **inputs, t_MemoryManager *manager);
t_TokenType				get_token_type(char *c);
int						get_token_type_from_pr(t_TokenInfo *previous_token);
int						create_token(t_TokenData *data, int type);
int						skip_spaces(char *input, int index);
int						handle_special_char(t_TokenData *data);

int						is_special_char(char c);
int						is_space(char c);
t_TokenNode				*arrayintonodes(t_TokenInfo *tokens,
							t_MemoryManager *manager);
t_TokenNode				*create_node(t_TokenInfo token,
							t_MemoryManager *manager);
void					print_linked_list(t_TokenNode *head);
void					free_linked_list(t_TokenNode *head);
int						ft_strcmp(const char *s1, const char *s2);

int						check_special_chars(t_TokenNode *head);
char					*add_spaces(char *input, t_MemoryManager *manager);
int						check_quotes(t_TokenNode *head, char c);
int						check_quotes_spiclal_chars(t_TokenNode *head, char c);
char					**split_by_quots(char *input);
int						track_quots(char *input);
int						print_error(char *str);

void					fill_env_list(t_MemoryManager *manager, char **env,
							t_env *env_list);
void					print_env_list(t_env *env_list);
int						check_key_if_deja(t_env *env_list, char *var);
int						check_special_validity(t_TokenNode *head);
int						check_syntax_double_special_cha(t_TokenNode *head);

int						check_syntax_special_face_to_face(t_TokenNode *head);
char					*check_value_env(char *str, t_env *head,
							t_MemoryManager *manager);
void					remove_quotes_from_first_and_last(char *input);
int						execute_builtin(t_node *head, t_env **env_list,
							t_MemoryManager *manager);
char					*ft_strndup(t_MemoryManager *manager, char *s, int n);
void					add_env_node(t_env **current, char *key, char *value,
							t_MemoryManager *manager);
t_node					*convert_to_node_list(t_TokenNode *token_list,
							t_MemoryManager *manager);
void					print_node_list(t_node *node_list);
void					remove_closed(char *input);
void					remove_quotes_and_join(t_node *head,
							t_MemoryManager *manager);
int						check_key_from_env(t_env *env_list, char *key);
char					**get_key_value(t_MemoryManager *manager, char *var);
void					expansion_process(t_node **head, t_env *env_list,
							int exit_status, t_MemoryManager *gc);
void					remove_all_quotes_and_join(char *input);
int						ft_strlen1(char **str);
int						is_numeric(const char *str);
char					*remove_closed_quotes(const char *input,
							t_MemoryManager *manager);
char					*remove_all_quotes2(const char *str,
							t_MemoryManager *manager);

// expander functions

char					*get_env_value3(t_env *env_list, char *key);
int						ft_strlen1(char **str);
char					**resize_args(char **args, int new_size,
							t_MemoryManager *gc);
int						dollar_position(char *str);
void					remove_args(char **args, int i);
int						in_quotes(char *str, int i, char c);
int						dstrchr2(char *s, char c, int *i);
char					*dstrchr(char *s, char c, int *flag);
void					append_char(char c, char **new_arg,
							t_MemoryManager *gc);
char					*ft_charjoin(char *s, char c, t_MemoryManager *gc);
char					*extract_var_name(char *arg, int *j,
							t_MemoryManager *gc);
char					*get_variable_value(t_env *env_list, char *var_name,
							t_MemoryManager *gc);
void					append_expanded_value(char **new_arg, char *value,
							t_MemoryManager *gc);
int						is_last_dollar(char *str, char c);
void					expand_variable(char *arg, char **new_arg, int *j,
							t_exec_context *context);
void					update_args(char **args, char *new_arg, int i);
int						get_arg_size(char **args);
void					shift_args(char **args, int i, int original_size,
							int num_splits);
void					split_and_remove_quotes(char ***args, char **split_args,
							int i, int num_splits);
void					handle_splitting(char ***args, int i,
							t_MemoryManager *gc);
void					handle_exit_status(char **new_arg, int *j,
							int exit_status, t_MemoryManager *gc);
void					handle_expansion(char **args, int i,
							t_exec_context *context, int *k);
int						is_dollar_only(char *str);
int						is_qouted(char *str);
void					change_qoutes1(char *str);
void					change_qoutes(char *str);
int						double_quotes(const char *str);
void					process_arguments(t_command *current_command,
							t_exec_context *context);
void					exp_reddd(char **args, t_env *env_list, int exit_status,
							t_MemoryManager *gc);
void					expand_redirect(t_redirect **redirect, t_env *env_list,
							int exit_status, t_MemoryManager *gc);
int						need_expansion(char *str);
void					expan_herdoc(char **args, t_env *env_list,
							int exit_status, t_MemoryManager *gc);
int						should_split_argument(char **args, int i, char *last);

// builtins functions:

int						is_builtin(char *command);
char					*get_oldpwd_path(t_env **env_list);
char					*handle_home_shortcut(char *path, t_env **env_list,
							t_MemoryManager *manager);
int						ft_export(t_command *command, t_env **env_list,
							t_MemoryManager *manager);
int						handle_export_arg(t_MemoryManager *manager, char *arg,
							t_env **env_list);
char					*get_env_value(t_env *env_list, char *key);
int						ft_echo(t_command *command);
int						ft_pwd(t_env *env_list, t_MemoryManager *gc);
int						ft_env(t_command *command, t_env *env_list);
int						ft_exit(t_command *command, t_MemoryManager *manager);
int						ft_unset(t_command *command, t_env **env_list,
							t_MemoryManager *manager);
int						ft_cd(t_command *command, t_env **env_list,
							t_MemoryManager *manager);
void					replace_env_value(t_env **env_list, char *key,
							char *value, t_MemoryManager *manager);

// utils functions:
int						wildcard_match(char *pattern, char *str);
void					print_error11(char *command, char *error);
void					ft_waitpid(pid_t last_pid, int *exit_status);
void					my_exit(int status, t_MemoryManager *manager);
int						is_plus_equal_case(char *arg);
void					add_new_env_entry(t_MemoryManager *manager, char *key,
							char *value, t_env **env_list);
char					**get_key_value_for_plus(t_MemoryManager *manager,
							char *var);
int						is_special_char(char c);
int						is_space(char c);
int						skip_spaces(char *input, int index);
int						is_quote(char c);
int						is_quote2(char c);
int						ret_check(t_TokenNode *current);
int						is_special_char2(char c);
int						is_wildcard(char *str);
int						is_wildcard_in_args(char **args);
int						is_space1(char *input);
int						validate_input(char *input, int *exit_status);
char					*read_user_input(void);
int						count_heredoc(t_TokenNode *list_head);
int						checking(t_TokenNode *list_head);
t_TokenInfo				*process_input(char *input, int *exit_status,
							t_MemoryManager *manager);
t_node					*prepare_execution(t_TokenInfo *tokens, t_env *env_list,
							int exit_status, t_MemoryManager *manager);
void					increment_shlvl(t_env *env_list,
							t_MemoryManager *manager);

// execution functions v1:

void					replace_wildcard_in_args(t_node *head,
							t_MemoryManager *manager);
char					**resize_args(char **args, int new_size,
							t_MemoryManager *gc);
void					shift_args(char **args, int i, int original_size,
							int num_splits);
int						is_heredoc(t_node *node);
void					execute_heredoc(t_node *current, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);
void					execute_piped_commands(t_node *current,
							t_env **env_list, int *exit_status);
void					execute_cmds(t_node *head, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);
int						is_redirection(t_node *node);
void					expand_redirect(t_redirect **redirect, t_env *env_list,
							int exit_status, t_MemoryManager *gc);
// execution functions v2:

char					*gett_env_value(const char *key, t_env *env_list);
char					*find_executable_in_path(char *command, t_env *env_list,
							t_MemoryManager *manager);
int						check_file_permissions(char *file);
int						execute_command(char *executable_path, char **args,
							t_env *env_list, t_MemoryManager *manager);
int						execute_external(t_command *command, t_env *env_list,
							t_MemoryManager *manager);
void					expan_herdoc(char **args, t_env *env_list,
							int exit_status, t_MemoryManager *gc);

// execution_v_1_1 functions

void					handle_builtin_command(t_node *node, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);
void					execute_external_command(t_node *node, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);
void					execute_single_command(t_node *node, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);
char					*find_executable(const char *command, char **paths,
							t_MemoryManager *gc);
int						count_env_variables(t_env *env_list);

// execution_v1_2 functions

char					*create_env_string(t_env *env, t_MemoryManager *gc);
char					**create_env_array(t_env *env_list,
							t_MemoryManager *gc);
int						count_path_components(const char *path);
char					**split_path(const char *path, t_MemoryManager *gc);

// signals functions
void					sig_child(void);
void					sig_herdoc(int sig);
void					sig_ignore(void);
void					sig_handler(int signo);
void					sigoo(t_node *m);
// heredocP1 functions

char					*create_temp_filename(t_MemoryManager *manager);
int						open_temp_file(const char *filename, int flags);
void					write_to_file(int fd, const char *str);
char					**process_heredoc_input(const char *delimiter,
							t_MemoryManager *gc);
void					handle_single_heredoc(t_redirect *redirect,
							const char *temp_file, t_exec_context *context);
// heredocP2 functions

void					execute_command_with_heredoc(t_node *temp,
							const char *temp_file, t_exec_context *context);
void					handle_heredoc(t_node *node, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);

// redirectionsP1 functions

void					handle_open_error(void);
void					handle_dup2_error(void);
int						open_file(const char *str, int flags);
void					redirect_output(const char *str, int flags);
void					redirect_input(const char *str);

// redirectionsP2 functions

void					handle_single_redirection(t_redirect *redirect);
int						handle_redirections(t_node *node, t_env **env_list,
							int *exit_status);
void					wait_for_children(pid_t last_pid, int *exit_status);

// execute_pipe_and_cmds functions

void					setup_pipe(t_process_data *proc_data);
void					handle_child_io(t_process_data *proc_data,
							t_node *current);
void					handle_child_process(t_process_data *proc_data,
							t_node *current, t_exec_context *ctx);
void					handle_parent_io(t_process_data *proc_data,
							t_node *current, pid_t pid);
void					handle_pipe_and_multiple_commands(t_node *head,
							t_exec_context *ctx);
int						is_heredoc(t_node *node);
int						is_exe_bult(t_node *node, t_env **env_list,
							int *exit_status, t_MemoryManager *gc);

// free functions

void					*gc_malloc(t_MemoryManager *manager, size_t size);
void					gc_free_all(t_MemoryManager *manager);
void					gc_free(t_MemoryManager *manager, void *ptr);
void					*gc_realloc(t_MemoryManager *manager, void *ptr,
							size_t new_size);

#endif
