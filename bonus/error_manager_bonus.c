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

void    error_failure(char *msg, int return_flag)
{
        perror(msg);
        if (return_flag == 127)
                exit(127);
        else
                exit(EXIT_FAILURE);
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

void	wait_for_child(pid_t pid, int fd1, int fd2, int fd3)
{
	int	status;

	while (waitpid(pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 127)
			{
				close_fd(fd1, fd2, fd3);
				exit(127);
			}
			if (WEXITSTATUS(status) == 1)
			{
				close_fd(fd1, fd2, fd3);
				exit(1);
			}
		}
	}
}
