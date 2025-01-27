/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:54:20 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

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

void	close_fd(int fd1, int fd2, int fd3)
{
	if (fd1 > 2)
		close(fd1);
	if (fd2 > 2)
		close(fd2);
	if (fd3 > 2)
		close(fd1);
}

void	fd_error(char *msg, int fd1, int fd2, int fd3)
{
	close_fd(fd1, fd2, fd3);
	perror(msg);
	exit(1);
}

void	wait_for_child(pid_t pid, int fd_to_close)
{
	int	status;

	while (waitpid(pid, &status, 0) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
			fd_error("Exit programm", fd_to_close, -1, -1);
	}
}
