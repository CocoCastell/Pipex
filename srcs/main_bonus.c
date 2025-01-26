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

#include "../includes/pipex.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void	command_execution(char **command, char **envp)
{
	char	*path;
	
	path = get_path(command, envp);
	if (path == NULL)
		error_free(command, NULL, NULL, "Command not found");
	if (execve((const char *)path, command, envp) == -1)
		error_free(command, path, NULL, "Execution error");
}

void	recursion(char *argv[], char *envp[], int pipe_fd[2], int data[2])
{
	char **command;
	int	file_fd = -1;
	int	new_pipe_fd[2];
	int	pid;
	int	status;

	ft_printf("argc: %d i: %d\n", data[0], data[1]);
	if (data[1] == data[0] - 2)
	{
		ft_printf("data: %d\n", data[1]);
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[0], -1);
		close(pipe_fd[0]);
		file_fd = open(argv[data[0] - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (file_fd == -1)
			fd_error("Error opening second file", -1, -1);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			fd_error("Error file dup2", file_fd, -1);
		close(file_fd);
		command = ft_split(argv[data[1]], ' ');
		command_execution(command, envp);
	}
	else
	{
		pipe(new_pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			fd_error(NULL, new_pipe_fd[0], new_pipe_fd[1]);
			fd_error("Fork error", pipe_fd[0], -1);
		}
		if (pid == 0)
		{
			close(new_pipe_fd[0]);
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
				fd_error("Error dup2", pipe_fd[0], new_pipe_fd[1]);
			close(pipe_fd[0]);
			if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
				fd_error("Error dup2", new_pipe_fd[1], -1);
			close(new_pipe_fd[1]);
			command = ft_split(argv[data[1]], ' ');
			command_execution(command, envp);
		}
		else
		{
			close(pipe_fd[0]);
			close(new_pipe_fd[1]);
			while (waitpid(pid, &status, 0) > 0)
			{
				if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
				{
					if (data[1] == 2)
						fd_error(NULL, new_pipe_fd[0], new_pipe_fd[1]);
					fd_error("Exit programm with status 2", pipe_fd[1], -1);
				}
			}
			data[1]++;
			recursion(argv, envp, new_pipe_fd, data);
		}
	}
}

void	pre_recursion(char *argv[], char *envp[], int data[2])
{
	int	pipe_fd[2];
	int	pid;
	int	status;
	char	**command;

	if (pipe(pipe_fd) == -1)
		fd_error("Pipe error", -1, -1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		command = ft_split(argv[data[1]], ' ');
		command_execution(command, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(pipe_fd[1]);
		recursion(argv, envp, pipe_fd, data);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int	infile_fd;
	int	data[2];

	if (argc >= 4)
	{
		infile_fd = open(argv[1], O_RDONLY);
		if (infile_fd == -1)
			fd_error("Error opening first file", -1, -1);
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			fd_error("Error dup2", infile_fd, -1);
		data[0] = argc;
		data[1] = 2;
		pre_recursion(argv, envp, data);
	}
	else
		return (ft_printf("Wrong number of arguments\n"), 1);
	return (0);
}
