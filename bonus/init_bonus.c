/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/29 16:12:55 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void    file_init(int argc, char *argv[], char *envp[])
{
        int     infile_fd;
        int     data[3];

        infile_fd = open(argv[1], O_RDONLY);
        if (infile_fd == -1)
		{
			perror("Error opening file");
			return ;
		}
        if (dup2(infile_fd, STDIN_FILENO) == -1)
                fd_error("Error dup2", infile_fd, -1, -1);
        close(infile_fd);
		data[0] = argc;
        data[1] = 2;
        data[2] = 0;
        pre_recursion(argv, envp, data);
}

void    here_doc_init(int argc, char *argv[], char *envp[])
{
        int     data[3];
        int     pipe_fd[2];
        char    *string;
        size_t  string_length;

        data[0] = argc;
        data[1] = 3;
        data[2] = 1;
        if (pipe(pipe_fd) == -1)
                fd_error("Pipe error", -1, -1, -1);
        while (1)
        {
                string = get_next_line(0);
                string_length = ft_strlen(string);
                if (ft_strncmp(argv[2], string, --string_length) == 0)
                        break ;
                write(pipe_fd[1], string, sizeof(char) * ++string_length);
        }
        close(pipe_fd[1]);
        pipex_recursion(argv, envp, pipe_fd, data);
}
