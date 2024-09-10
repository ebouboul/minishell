/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouboul <ebouboul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 19:03:28 by ebouboul          #+#    #+#             */
/*   Updated: 2024/09/10 11:32:20 by ebouboul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>   // Include for DIR, opendir, readdir, closedir
#include <fnmatch.h>  // Include for fnmatch
// #include <string.h>   // Include for strerror

int is_wildcard(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '*')
            return 1;
        i++;
    }
    return 0;
}

int is_wildcard_in_args(char **args)
{
    int i = 0;
    while (args[i] != NULL)
    {
        if (is_wildcard(args[i]))
            return 1;
        i++;
    }
    return 0;
}


int wildcard_match(const char *pattern, const char *str)
{
    int pIndex = 0;                // Pattern index
    int sIndex = 0;                // String index
    int backtrackPIndex = -1;      // Last '*' position in the pattern
    int backtrackSIndex = -1;      // Corresponding position in the string to backtrack to

    // Ignore hidden files that start with '.'
    if (str[0] == '.')
    {
        return 0;  // Hidden file, no match
    }

    while (str[sIndex] != '\0')    // Iterate through the string
    {
        if (pattern[pIndex] == '*') // If current pattern character is '*'
        {
            // Mark the position of '*' and the corresponding position in the string
            backtrackPIndex = pIndex;
            backtrackSIndex = sIndex;
            pIndex++; // Move to the next character in the pattern
        }
        else if (pattern[pIndex] == str[sIndex] ) // Direct match or single character match ('?')
        {
            pIndex++; // Move to the next pattern character
            sIndex++; // Move to the next string character
        }
        else if (backtrackPIndex != -1) // Backtrack if there was a '*' previously
        {
            pIndex = backtrackPIndex + 1; // Reset to the position after the last '*'
            sIndex = ++backtrackSIndex;   // Move to the next character in the string
        }
        else
        {
            return 0; // No match possible
        }
    }

    // Handle any remaining '*' in the pattern
    while (pattern[pIndex] == '*')
    {
        pIndex++;
    }

    // Return true if both the pattern and the string have been fully matched
    return (pattern[pIndex] == '\0');
}


void replace_wildcard(char ***args, int *i)
{
    char *path = (*args)[*i];
    struct dirent *entry;
    DIR *dir = opendir(".");
    int match_count = 0;
    char **matches = NULL;

    if (dir == NULL)
    {
        printf("minishell: %s: %s\n", path, strerror(1));
        return;
    }

    // Find all matches using our custom wildcard_match function
    while ((entry = readdir(dir)) != NULL)
    {
        if (wildcard_match(path, entry->d_name))
        {
            match_count++;
            matches = realloc(matches, match_count * sizeof(char *));
            matches[match_count - 1] = ft_strdup(entry->d_name);
        }
    }
    closedir(dir);

    if (match_count == 0)
        return; // No matches found

    // Resize the args array to accommodate the new matches
    int old_size = ft_strlen1(*args);
    int new_size = old_size + match_count - 1;
    *args = resize_args(*args, new_size);

    // Shift arguments to make room for the new matches
    shift_args(*args, *i, old_size, match_count);

    // Insert the new matches
    for (int j = 0; j < match_count; j++)
    {
        // gc_free((*args)[*i + j]);
        (*args)[*i + j] = matches[j];
    }

    gc_free(matches);
}



// void replace_wildcard(char ***args, int *i)
// {
//     char *path = (*args)[*i];
//     struct dirent *entry;
//     DIR *dir = opendir(".");
//     int match_count = 0;
//     char **matches = NULL;

//     if (dir == NULL)
//     {
//         printf("minishell: %s: %s\n", path, strerror(1));
//         return;
//     }

//     // Find all matches
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (fnmatch(path, entry->d_name, FNM_PATHNAME) == 0)
//         {
//             match_count++;
//             matches = realloc(matches, match_count * sizeof(char *));
//             matches[match_count - 1] = ft_strdup(entry->d_name);
//         }
//     }
//     closedir(dir);

//     if (match_count == 0)
//         return; // No matches found

//     // Resize the args array to accommodate the new matches
//     int old_size = ft_strlen1(*args);
//     int new_size = old_size + match_count - 1;
//     *args = resize_args(*args, new_size);
    

//     // Shift arguments to make room for the new matches
//     shift_args(*args, *i, old_size, match_count);

//     // Insert the new matches
//     int j = 0;
//     while (j < match_count)
//     {
//         // free((*args)[*i + j]);
//         (*args)[*i + j] = matches[j];
//         j++;
//     }

//     gc_free(matches);
// }


void replace_wildcard_in_args(t_node *head)
{
    t_node *current = head;
    while(current != NULL)
    {
        if (is_wildcard_in_args(current->command->args))
        {
            int i = 0;
            while (current->command->args[i] != NULL)
            {
                if (is_wildcard(current->command->args[i]))
                {
                    replace_wildcard(&current->command->args, &i);
                }
                i++;
            }
        }
        current = current->next;
    }
}
