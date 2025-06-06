/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/30 14:00:06 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include "../libft/libft_b/includes/libft.h"
# include "../libft/gnl_b/includes/get_next_line_bonus.h"
# include "../libft/ft_printf_b/includes/ft_printf_bonus.h"

void	free_array(char **command, char **path_env, char *path);
void	*get_path(char **command, char **envp);
void	fd_error(char *msg, int fd1, int fd2);
void	*find_path(char **path, char **command);
void	command_execution(char **command, char **envp);
void	child_process(char *argv[], char **envp, int pipe_fd[2]);
void	error_failure(char *msg, int return_flag);
void	parent_process(char *argv[], char **envp, int pipe_fd[2]);

#endif
