/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:23:01 by ebouboul          #+#    #+#             */
/*   Updated: 2024/08/25 22:27:14 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_command *convert_to_command(TokenNode **current) {
    t_command *cmd = malloc(sizeof(t_command));
    cmd->args = malloc(sizeof(char *) * MAX_TOKENS); 
    cmd->redirect = NULL;
    cmd->next = NULL;

    int arg_count = 0;
    while (*current && (*current)->info.type != TOKEN_PIPE) {
        if ((*current)->info.type == TOKEN_COMMAND || (*current)->info.type == TOKEN_ARG) {
            cmd->args[arg_count++] = strdup((*current)->info.value);
        } else if ((*current)->info.type == TOKEN_REDIRECT_IN || (*current)->info.type == TOKEN_REDIRECT_OUT ||
                   (*current)->info.type == TOKEN_APPEND || (*current)->info.type == TOKEN_HEREDOC) {
            t_redirect *redir = malloc(sizeof(t_redirect));
            redir->str = strdup((*current)->next->info.value);
            redir->flag = (*current)->info.type;  
            redir->next = cmd->redirect;
            cmd->redirect = redir;
            *current = (*current)->next;  
        }
        
        *current = (*current)->next;
    }
    cmd->args[arg_count] = NULL;  
    return cmd;
}


t_node *convert_to_node_list(TokenNode *token_list) {
    t_node *node_list = NULL;
    t_node *last_node = NULL;

    while (token_list) {
        t_node *new_node = malloc(sizeof(t_node));
        new_node->command = convert_to_command(&token_list);
        new_node->exit_status = 0;  
        new_node->next = NULL;

        if (!node_list) {
            node_list = new_node;
        } else {
            last_node->next = new_node;
        }
        last_node = new_node;

        if (token_list && token_list->info.type == TOKEN_PIPE) {
            token_list = token_list->next;
        }
    }
    return node_list;
}

void print_node_list(t_node *node_list) 
{
    t_node *current = node_list;
    while (current != NULL) {
        int i = 0; 
        while (current->command->args && current->command->args[i]) {
            printf("Arg: %s\n", current->command->args[i]);
            i++;
        }
        
        t_redirect *redir = current->command->redirect;
        while (redir != NULL) {
            printf("Redirect: %s, Type: %d\n", redir->str, redir->flag);
            redir = redir->next;
        }
        if (current->next) {
            printf("next\n");
        }
        
        current = current->next;
    }
}
