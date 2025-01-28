/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 20:31:42 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// data[0] -> argc
// data[1] -> argument index
// data[2] -> here_doc option (1) or not (0)

void	bonus_child_process(char *argv[], char *envp[], int new_pipe_fd[2], int data[3])
{
	close(new_pipe_fd[0]);
	if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
		fd_error("Error dup2", new_pipe_fd[1], -1, -1);
	close(new_pipe_fd[1]);
	command_execution(argv, envp, data);
}

void	bonus_parent_process(char *argv[], char *envp[], int pipe_fd[2], int data[3])
{
	int	new_pipe_fd[2];
	pid_t	pid;
	
	if (pipe(new_pipe_fd) == -1)
		fd_error("Pipe error", pipe_fd[0], -1, -1);
	pid = fork();
	if (pid == -1)
			fd_error("Fork error", new_pipe_fd[0], new_pipe_fd[1], pipe_fd[0]);
	if (pid == 0)
	{
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[0], new_pipe_fd[1], new_pipe_fd[0]);
		close(pipe_fd[0]);
		bonus_child_process(argv, envp, new_pipe_fd, data);
	}
	else
	{
		if (data[1] > 3)
			close(pipe_fd[1]);
		wait_for_child(pid, new_pipe_fd[0], new_pipe_fd[1], pipe_fd[0]);
		close_fd(pipe_fd[0], new_pipe_fd[1], -1);
		data[1]++;
		pipex_recursion(argv, envp, new_pipe_fd, data);
	}
}

void	pipex_recursion(char *argv[], char *envp[], int pipe_fd[2], int data[3])
{
	if (data[1] == data[0] - 2)
		outfile_command(argv, envp, pipe_fd, data);
	else
		bonus_parent_process(argv, envp, pipe_fd, data);
}

void	pre_recursion(char *argv[], char *envp[], int data[3])
{
	int	pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		fd_error("Pipe error", -1, -1, -1);
	pid = fork();
	if (pid == -1)
		fd_error("Fork error", pipe_fd[0], pipe_fd[1], -1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			fd_error("Dup2 error", pipe_fd[1], -1, -1);
		close(pipe_fd[1]);
		command_execution(argv, envp, data);
	}
	else
	{	
		wait_for_child(pid, pipe_fd[0], pipe_fd[1], -1);
		close(pipe_fd[1]);
		data[1]++;
		pipex_recursion(argv, envp, pipe_fd, data);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	if (envp == NULL)
		return (ft_printf("Variable environment error\n"), 1);
	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
			here_doc_init(argc, argv, envp);
		else
			file_init(argc, argv, envp);
	}
	else
		return (ft_printf("Wrong number of arguments\n"), 0);
	return (0);
}
