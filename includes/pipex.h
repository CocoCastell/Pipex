/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/24 12:04:18 by cochatel         ###   ########.fr       */
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

void	simple_error(char *msg);
void	*get_path(char **command, char **envp);
void	*find_path(char **path, char **command);
void	child_process(char *argv[], char **envp, int pipe_fd[2]);
void	parent_process(char *argv[], char **envp, int pipe_fd[2]);
void	error_free(char **command, char *path, char **path_env, char *msg);

#endif
