#include "minishell.h"

char* check_value_env(char *str, t_env *head)
{
    t_env *current = head;
    char *val;
    while(current != NULL)
    {
        if(strcmp(current->env->key, str +1 ) == 0)
        {
            printf("%s\n", current->env->key);
            val = current->env->value;
            return val;
        }
        current = current->next;
    }
    return NULL;
}
void expansion_process(TokenNode *head, t_env *key)
{
    TokenNode *current = head;
    (void) key;
    while(current)
    {
        if(current->info.value[0] == '$')
        {
            current->info.value = ft_strdup(check_value_env(current->info.value, key));
        }
        current = current->next;
    }
}