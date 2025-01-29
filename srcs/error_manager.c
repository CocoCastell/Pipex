/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/29 16:04:05 by cochatel         ###   ########.fr       */
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

void    fd_error(char *msg, int fd1, int fd2)
{
        if (fd1 > 2)
                close(fd1);
        if (fd2 > 2)
                close(fd2);
        perror(msg);
        exit(1);
}
