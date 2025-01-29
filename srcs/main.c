/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/29 19:09:31 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	command_execution(char **command, char **envp)
{
	char	*path;

	if (command[0] == NULL)
	{
		free_array(command, NULL, NULL);
		error_failure("Error: null pointeur", 1);
	}
	path = get_path(command, envp);
	if (path == NULL)
	{
		free_array(command, NULL, NULL);
		error_failure("Command not found", 127);
	}
	if (execve((const char *)path, command, envp) == -1)
	{
		free_array(command, NULL, path);
		error_failure("Execution error", 1);
	}
}

void	child_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	int		infile_fd;

	close(pipe_fd[0]);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		perror("Error opening first file");
		infile_fd = open("/dev/null", O_RDONLY);
		if (infile_fd == -1)
			fd_error("Error opening file", pipe_fd[1], -1);
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		fd_error("Dup2 error", infile_fd, -1);
	close(infile_fd);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		fd_error("Dup2 error", pipe_fd[1], -1);
	close(pipe_fd[1]);
	command = ft_split(argv[2], ' ');
	command_execution(command, envp);
}

void	parent_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	int		outfile_fd;

	close(pipe_fd[1]);
	outfile_fd = open(argv[4], O_RDONLY | O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile_fd == -1)
		fd_error("Error opening second file", pipe_fd[0], -1);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		fd_error("Dup2 error", outfile_fd, pipe_fd[0]);
	close(pipe_fd[0]);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		fd_error("Dup2 file error", outfile_fd, -1);
	close(outfile_fd);
	command = ft_split(argv[3], ' ');
	command_execution(command, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		return (ft_printf("Wrong number of arguments\n"), 1);
	if (envp == NULL)
		return (ft_printf("Variable environment error\n"), 1);
	if (pipe(fd) == -1)
		return (ft_printf("Pipe error\n"), 1);
	pid = fork();
	if (pid == -1)
		fd_error("Forking error", fd[0], fd[1]);
	if (pid == 0)
		child_process(argv, envp, fd);
	else
	{
		while (waitpid(pid, NULL, 0) > 0);
		parent_process(argv, envp, fd);
	}
	return (0);
}
