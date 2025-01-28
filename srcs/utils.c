/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:58:23 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    free_array(char **command, char **path_env, char *path)
{
        int     i;

        i = -1;
        if (path_env != NULL)
        {
                while (path_env[++i])
                        free(path_env[i]);
                free(path_env[i]);
                free(path_env);
        }
        i = -1;
        if (command != NULL)
        {
                while (command[++i])
                        free(command[i]);
                free(command[i]);
                free(command);
        }
        if (path != NULL)
                free(path);
}

void	*find_path(char **path, char **command)
{
	char	*full_path;	
	char	*half_path;
	int	i;

	i = 0;
	while (path[i] != NULL)
	{
		half_path = ft_strjoin(path[i], "/");
		if (half_path == NULL)
		{
			free_array(command, path, NULL);
			error_failure("Error: null pointer", 1);
		}
		full_path = ft_strjoin(half_path, command[0]);
		free(half_path);
		if (full_path == NULL)
		{
			free_array(command, path, NULL);
			error_failure("Error: null pointer", 1);
		}
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	*get_path(char **command, char **envp)
{
	char	**path;
	char	*full_path;
	int		i;

	i = 0;
	if (access(command[0], F_OK | X_OK) == 0)
		return (command[0]);
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	if (path == NULL)
	{
		free_array(command, path, NULL);
		error_failure("Error: null pointer", 1);
	}
	full_path = find_path(path, command);
	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
	return (full_path);
}
