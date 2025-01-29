/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/29 14:45:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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


// ERROR

void	close_fd(int fd1, int fd2, int fd3);
void	wait_for_child(pid_t pid, int fd1, int fd2, int fd3);
void	free_array(char **command, char **path_env, char *path);
void	fd_error(char *msg, int fd1, int fd2, int fd3);
void	error_failure(char *msg, int return_flag);

// ALGO

void	file_init(int argc, char *argv[], char *envp[]);
void	here_doc_init(int argc, char *argv[], char *envp[]);
void	*get_path(char **command, char **envp);
void	*find_path(char **path, char **command);
void	here_doc_init(int argc, char *argv[], char *envp[]);
void	pre_recursion(char *argv[], char *envp[], int data[3]);
void	command_execution(char *argv[], char **envp, int data[3]);
void	outfile_command(char *argv[], char *envp[], int pipe_fd[2], int data[3]);
void	pipex_recursion(char *argv[], char *envp[], int pipe_fd[2], int data[3]);
void	bonus_parent_process(char *argv[], char *envp[], int pipe_fd[2], int data[3]);
void	bonus_child_process(char *argv[], char *envp[], int new_pipe_fd[2], int data[3]);

#endif
