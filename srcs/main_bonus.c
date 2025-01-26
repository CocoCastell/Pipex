/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:32:14 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
	int file_fd = -1;
	int	new_pipe_fd[2];
	int	pid;

	if (data[1] + 1 >= data[0])
	{
		close(pipe_fd[1]);
		file_fd = open(argv[data[1]], O_WRONLY | O_CREAT, 0644);
		if (file_fd == -1)
			fd_error("Error opening second file", pipe_fd[0], -1);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[0], file_fd);
		close(file_fd);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[0], -1);
		close(pipe_fd[0]);
		command = ft_split(argv[data[1]], ' ');
		command_execution(command, envp);
	}
	else
	{
		if (pipe(new_pipe_fd) == -1)
			fd_error("Pipe error", pipe_fd[0], pipe_fd[1]);
		pid = fork();
		if (pid == -1)
		{
			fd_error(NULL, pipe_fd[0], pipe_fd[1]);
			fd_error("Error forking", new_pipe_fd[0], new_pipe_fd[1]);
		}
		if (pid == 0)
		{
			close(new_pipe_fd[0]);
			if (data[2] > 1)
			{
				close(pipe_fd[1]);
				if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
					fd_error("Error dup2", pipe_fd[0], new_pipe_fd[1]);
				close(pipe_fd[0]);
				if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
					fd_error("Error dup2", new_pipe_fd[1], -1);
			}
			else
			{
				close(pipe_fd[0]);
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
					fd_error("Error dup2", new_pipe_fd[1], pipe_fd[0]);
				close(pipe_fd[1]);
			}
			close(new_pipe_fd[1]);
			command = ft_split(argv[data[1]], ' ');
			command_execution(command, envp);
		}
		else
		{
			data[1] = data[1] + 1;
			recursion(argv, envp, new_pipe_fd, data);
		}
	}
}

/*void	child_process(char *full_command, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	*command_string;
	char	*path;
	
	if (pipe(pipe_fd) == -1)
		error;
	pid = fork();
	if (pid == -1)
		error;
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			errror;
		close(pipe_fd[1]);
		command_string = ft_split(full_command, ' ');
		command_execution(command_string, envp);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			error;
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
}*/

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	infile_fd;
	int	pipe_fd[2];
	int	data[2];
	//int	outfile_fd;

	if (argc >= 5)
	{
		i = 0;
		infile_fd = open(argv[1], O_RDONLY);
		if (infile_fd == -1)
			fd_error("Error opening first file", -1, -1);
	//	output_fd = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)
	//	if (output_fd == -1)
	//		fd_error("Error dup2", infile_fd, -1);
	//	if (dup2(infile_fd, STDIN_FILENO) == -1)
	//		fd_error("Error dup2", infile_fd, -1);
	//	while (i < argc)
	//		child_process(argv[++i]);
		if (pipe(pipe_fd) == -1)
			fd_error("Pipe error", infile_fd, -1);
		data[0] = argc;
		data[1] = 1;
		recursion(argv, envp, pipe_fd, data);
	}
	return (0);
}
