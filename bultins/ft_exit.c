/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:59:36 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/17 20:11:02 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_numeric(const char *str)
{
    int i = 0;
    while (str &&str[i] != '\0')
    {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
            return 0;
        i++;
    }
    return 1;
}

int ft_exit(t_command *command, MemoryManager *manager)
{
    int status = 0;
    if (ft_strlen1(command->args) > 2 && !is_numeric(command->args[1]))
    {
        if (is_numeric(command->args[1]))
            printf("exit: %s: numeric argument required\n", command->args[1]);
        else
        {
            printf("exit: too many arguments\n");
            return 1;
        }
    }
    if (command->args[1] != NULL)
    {
        status = ft_atoi(command->args[1]);
    }
    if (!is_numeric(command->args[1]))
    {
        printf("exit: %s: numeric argument required\n", command->args[1]);
        return 1;
    }
    gc_free_all(manager);
    exit(status);
}