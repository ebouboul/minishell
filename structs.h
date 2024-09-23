/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 03:06:22 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/22 21:53:51 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <sys/types.h>

typedef struct t_MemoryNode
{
	void				*ptr;
	struct t_MemoryNode	*next;
}						t_MemoryNode;
typedef struct t_MemoryManager
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

#endif