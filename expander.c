/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:13:42 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/15 22:07:40 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value3(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->env->key, key) == 0)
			return (current->env->value);
		current = current->next;
	}
	return ("");
}

int	ft_strlen1(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		i++;
	}
	return (i);
}

char	**resize_args(char **args, int new_size)
{
	char	**new_args;
	int		i;

	new_args = (char **)gc_malloc(sizeof(char *) * (new_size + 1));
	i = 0;
	if (new_args == NULL)
		return (NULL);
	while (args[i] != NULL)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = NULL;
	gc_free(args);
	return (new_args);
}
int	dollar_position(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

void	remove_args(char **args, int i)
{
	int	j;

	j = i;
	while (args[j] != NULL)
	{
		args[j] = args[j + 1];
		j++;
	}
}
int	in_quotes(char *str, int i, char c)
{
	int	flag;

	flag = 0;
	while (i >= 0)
	{
		if (str[i] == '\"')
		{
			i--;
			while (i >= 0 && str[i] != '\"')
			{
				if (str[i] == c)
					flag = 1;
				i--;
			}
			if (i < 0 && flag == 1)
				return (0);
			else
				i--;
			flag = 0;
		}
		else
			i--;
	}
	return (1);
}
int	dstrchr2(char *s, char c, int *i)
{
	char	quote;

	(*i)++;
	quote = s[*i - 1];
	while (s[*i] && s[*i] != quote)
	{
		if (s[*i] == c && s[*i + 1] != '\0' && quote == '\"')
			return (1);
		(*i)++;
	}
	if (s[*i] == quote)
		(*i)++;
	return (0);
}
char	*dstrchr(char *s, char c, int *flag)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if ((s[i] == '\"' && ft_strchr(&s[i + 1], '\"')) || s[i] == '\''
			|| !in_quotes(s, (ft_strlen(s) - 1), c))
		{
			if (s[i] != '\"' && s[i] != '\'')
			{
				while (s[i] && s[i] != c && s[i] != '\"')
					i++;
				if (s[i] == c)
					return (*flag = 1, &s[i]);
			}
			else if (dstrchr2(s, c, &i))
				return (*flag = 1, &s[i]);
		}
		else if (s[i] == c)
			return (*flag = i, &s[i]);
		else
			i++;
	}
	return (NULL);
}

void	append_char(char c, char **new_arg)
{
	char	temp[2] = {c, '\0'};
	char	*old_arg;

	old_arg = *new_arg;
	*new_arg = ft_strjoin(old_arg, temp);
	gc_free(old_arg);
}

void	handle_single_quote(char *arg, char **new_arg, int *in_single_quotes,
		int *j)
{
	if (new_arg == NULL)
	{
		*new_arg = ft_strdup("");
	}
	*in_single_quotes = !(*in_single_quotes);
	append_char(arg[(*j)++], new_arg);
}

void	handle_double_quote(char *arg, char **new_arg, int *in_double_quotes,
		int *j)
{
	if (new_arg == NULL)
	{
		*new_arg = ft_strdup("");
	}
	*in_double_quotes = !(*in_double_quotes);
	append_char(arg[(*j)++], new_arg);
}

char	*ft_charjoin(char *s, char c)
{
	char	*result;
	int		i;
	int		len;

	len = ft_strlen(s);
	result = (char *)gc_malloc(len + 2);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		result[i] = s[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}
int	is_quot1e(char c)
{
	return (c == '\'' || c == '"');
}

char	*extract_var_name(char *arg, int *j)
{
	int		k;
	char	*var_name;

	k = *j;
	if (arg[k] == '$' && arg[k + 1] == '$')
	{
		*j = k + 2;
		return (ft_strdup("$"));
	}
	while (arg[k] && (isalpha(arg[k + 1]) || isdigit(arg[k + 1]) || arg[k
			+ 1] == '_'))
		k++;
	if (ft_strchr(ft_substr(arg, *j, k), '\'') != NULL
			&& ft_strchr(ft_substr(arg, *j, k), '"') == NULL
			&& ft_strchr(ft_substr(arg, *j, k + 1), '\'')[1] != '"')
		return (NULL);
	var_name = ft_substr(arg, *j + 1, k - *j);
	if (!var_name)
		return (NULL);
	*j = k + 1;
	return (var_name);
}

char	*get_variable_value(t_env *env_list, char *var_name)
{
	char	*value;

	if (isdigit(var_name[0]))
		value = ft_strdup(var_name + 1);
	else
		value = get_env_value3(env_list, var_name);
	if (!value)
		return (NULL);
	return (value);
}

void	append_expanded_value(char **new_arg, char *value)
{
	char	*temp;

	temp = *new_arg;
	*new_arg = ft_strjoin(temp, value);
	if (!*new_arg)
		return ;
	gc_free(temp);
}

int	is_last_dollar(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	if (str[i - 1] == c)
		return (1);
	return (0);
}
void	expand_variable(char *arg, char **new_arg, int *j, t_env *env_list,
		int *k)
{
	char	*var_name;
	char	*value;

	if (arg == NULL || *j >= (int)ft_strlen(arg))
		return ;
	var_name = NULL;
	value = NULL;
	var_name = extract_var_name(arg, j);
	if (var_name)
	{
		value = get_variable_value(env_list, var_name);
		if (value)
			append_expanded_value(new_arg, value);
		if (value[0] != '\0')
		*k = *j;
	}
	if (*j < (int)ft_strlen(arg))
		append_char(arg[(*j)++], new_arg);
}

void	update_args(char **args, char *new_arg, int i)
{
	if (new_arg[0] != '\0')
		args[i] = new_arg;
	else
		remove_args(args, i);
}

int	get_arg_size(char **args)
{
	int	size;

	size = 0;
	while (args[size] != NULL)
		size++;
	return (size);
}

void	shift_args(char **args, int i, int original_size, int num_splits)
{
	int	k;

	k = original_size + num_splits - 1;
	while (k >= i + num_splits)
	{
		args[k] = args[k - num_splits + 1];
		k--;
	}
}

void	split_and_remove_quotes(char ***args, char **split_args, int i,
		int num_splits)
{
	int	k;

	k = 0;
	while (k < num_splits)
	{
		(*args)[i + k] = split_args[k];
		k++;
	}
}

void	handle_splitting(char ***args, int i)
{
	char	**split_args;
	int		num_splits;
	int		original_size;

	split_args = ft_split3((*args)[i], ' ');
	original_size = get_arg_size(*args);
	num_splits = get_arg_size(split_args);
	*args = resize_args(*args, original_size + num_splits - 1);
	shift_args(*args, i, original_size, num_splits);
	split_and_remove_quotes(args, split_args, i, num_splits);
	gc_free(split_args);
}
int	is_single_qupte_after_dollar(char *str)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			flag = 1;
		i++;
	}
	if (flag == 1 && ft_strchr(str, '"') == NULL)
		return (1);
	return (0);
}

int	no_expansion_needed(char *arg)
{
	int	k;

	return (ft_strchr(arg, '$') == NULL || dollar_position(arg) == 0
		|| is_single_qupte_after_dollar(arg) || !dstrchr(arg, '$', &k));
}
void	handle_exit_status(char **new_arg, int *j, int exit_status)
{
	char	*value;

	*new_arg = ft_strdup("");
	value = ft_itoa(exit_status);
	append_expanded_value(new_arg, value);
	*j += 2;
}
void	handle_expansion(char **args, int i, t_env *env_list, int exit_status,
		int *k)
{
	char	*new_arg;

	int(j), (in_single_quotes), (in_double_quotes);
	j = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	new_arg = ft_strdup("");
	while (args[i][j] != '\0')
	{
		if (args[i][j] == '\'' && !in_double_quotes)
			handle_single_quote(args[i], &new_arg, &in_single_quotes, &j);
		else if (args[i][j] == '"' && !in_single_quotes)
			handle_double_quote(args[i], &new_arg, &in_double_quotes, &j);
		else if (!in_single_quotes && args[i][j] == '$' && args[i][j + 1] != '\0')
		{
			if (args[i][j + 1] == '?')
				handle_exit_status(&new_arg, &j, exit_status);
			else
				expand_variable(args[i], &new_arg, &j, env_list, k);
		}
		else if (j < (int)ft_strlen(args[i]))
		{
			append_char(args[i][j++], &new_arg);
			*k = j;
			
		}
	}
	update_args(args, new_arg, i);
}
int	is_dollar_only(char *str)
{
	if ((str[0] == '$' && str[1] == '\0' ))
		return (1);
	return (0);
}
int	is_qouted(char *str)
{
	if (str[ft_strlen(str) - 1] == '\"' || str[ft_strlen(str) - 1] == '\'')
		return (1);
	return (0);
}
void change_qoutes1(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\"' )
			str[i] = 15;
		if (str[i] == '\'')
			str[i] = 16;
		i++;
	}
}

void change_qoutes(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\"' && i != 0 && i != (int)ft_strlen(str) - 1)
			str[i] = 15;
		if (str[i] == '\''&& i != 0 && i != (int)ft_strlen(str) - 1)
			str[i] = 16;
		i++;
	}
}

void	process_arguments(t_command *current_command, t_env *env_list,
		int exit_status)
{
	char(**args), (*last);
	int(i), (j), (k);
	args = current_command->args;
	i = 0;
	while (args[i] != NULL)
	{
		last = args[i];
		k = 0;
		while (last && dstrchr(last, '$', &j) && !is_dollar_only(last))
		{
			handle_expansion(args, i, env_list, exit_status, &k);
			if (args[i] && ft_strchr(args[i], ' ') != NULL 
				&& ft_strncmp(args[0], "export", 6) != 0
				&& is_last_dollar(args[i], '$') == 0&& !is_qouted(args[i]))
				{
					(handle_splitting(&args, i)), (change_qoutes1(args[i])); 
					break;
				}
			change_qoutes(args[i]);
			if (k >= (int)ft_strlen(args[i]))
				break ;
			last = args[i] + k ;
		}
		i++;
	}
	current_command->args = args;
}

void	expansion_process(t_node **head, t_env *env_list, int exit_status)
{
	t_node *current;
	t_command *current_command;

	current = *head;
	while (current != NULL)
	{
		current_command = current->command;
		while (current_command != NULL)
		{
			process_arguments(current_command, env_list, exit_status);
			current_command = current_command->next;
		}
		current = current->next;
	}
}