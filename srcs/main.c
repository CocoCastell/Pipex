/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/24 16:05:24 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	char	*path;
	int		infile_fd;
	
	close(pipe_fd[0]);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
		simple_error("Error opening first file");
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		simple_error("Dup2 error");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		simple_error("Dup2 error");
	close(pipe_fd[1]);
	command = ft_split(argv[2], ' ');
	path = get_path(command, envp);
	if (path == NULL)
		error_free(command, NULL, NULL, "Second command not found");
	close(infile_fd);
	if (execve((const char *)path, command, envp) == -1)
		error_free(command, path, NULL, "Execution error");
}

void	parent_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	char	*path;
	int		outfile_fd;

	outfile_fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile_fd == -1)
		simple_error("Error opening second file");
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		simple_error("Dup2 error");
	close(pipe_fd[0]);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		simple_error("Dup2 errror");
	command = ft_split(argv[3], ' ');
	path = get_path(command, envp);
	if (path == NULL)
		error_free(command, NULL, NULL, "Second command not found");
	close(outfile_fd);
	if (execve((const char *)path, command, envp) == -1)
		error_free(command, path, NULL, "Execution error");
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd[2];
	int		status;
	pid_t	pid;
	
	if (argc != 5)
		return (ft_printf("Wrong number of arguments\n"), 1);
	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		child_process(argv, envp, fd);
	else
	{	
		close(fd[1]);
		if (waitpid(pid, &status, WNOHANG) == -1)
			simple_error("Error with child processus:");
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 2)
				return (1);
		}
		parent_process(argv, envp, fd);
	}
	return (0);
}
