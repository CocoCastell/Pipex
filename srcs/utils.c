/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/24 15:56:33 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	error_free(char **command, char *path, char **path_env, char *msg)
{
	int	i;

	i = -1;
	if (path_env != NULL)
	{
		while (path_env[++i])
			free(path_env[i]);
		free(path_env[i]);
	}
	i = -1;
	while (command[++i])
		free(command[i]);
	free(command);
	if (path != NULL)
		free(path);
	perror(msg);
	exit(2);
}

void	fd_error(char *msg, int fd1, int fd2)
{
	if (fd1 > 2)
		close(fd1);
	if (fd2 > 2)
		close(fd2);
	perror(msg);
	exit(1);
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
			error_free(command, NULL, path, "memory allocation failed\n");
		full_path = ft_strjoin(half_path, command[0]);
		free(half_path);
		if (full_path == NULL)
			error_free(command, NULL, path, "memory allocation failed\n");
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
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	if (envp[i] == NULL)
		error_free(command, NULL, NULL, "Command not found");
	path = ft_split(envp[i] + 5, ':');
	if (path == NULL)
		error_free(command, NULL, NULL, "Memory allocation failed");
	full_path = find_path(path, command);
	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
	return (full_path);
}
