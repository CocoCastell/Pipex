/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:50:36 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    error_failure(char *msg, int return_flag)
{
        perror(msg);
        if (return_flag == 127)
                exit(127);
        else
                exit(EXIT_FAILURE);
}

void    wait_for_child(pid_t pid, int fd1_to_close, int fd2_to_close)
{
        int     status;

        while (waitpid(pid, &status, 0) > 0)
        {
                if (WIFEXITED(status))
                {
                        if (WEXITSTATUS(status) == 127)
                        {
                                close(fd1_to_close);
                                close(fd2_to_close);
                                exit(127);
                        }
                        if (WEXITSTATUS(status) == 1)
                        {
                                close(fd1_to_close);
                                close(fd2_to_close);
                                exit(EXIT_FAILURE);
                        }
                }
        }
}

void    fd_error(char *msg, int fd1, int fd2)
{
        if (fd1 > 2)
                close(fd1);
        if (fd2 > 2)
                close(fd2);
        perror(msg);
        exit(1);
}
