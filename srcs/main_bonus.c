/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/24 16:05:24 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	command_execution(char **command, char **envp)
{
	char	*path;
	
	path = get_path(command, envp);
	if (path == NULL)
		error_free(command, NULL, NULL, "Command not found");
	if (execve((const char *)path, command, envp) == -1)
		error_free(command, path, NULL, "Execution error");
}

void    child_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	int		infile_fd;

	close(pipe_fd[0]);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
		fd_error("Error opening first file", pipe_fd[1], -1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		fd_error("Dup2 error", infile_fd, pipe_fd[1]);
	close(pipe_fd[1]);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		fd_error("Dup2 error", infile_fd, -1);
	close(infile_fd);
	command = ft_split(argv[2], ' ');
	command_execution(command, envp);
}

void    parent_process(char *argv[], char **envp, int pipe_fd[2])
{
	char	**command;
	int		outfile_fd;

	outfile_fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile_fd == -1)
		fd_error("Error opening second file", pipe_fd[0], -1);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		fd_error("Dup2 error", outfile_fd, pipe_fd[0]);
	close(pipe_fd[0]);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		fd_error("Dup2 errror", outfile_fd, -1);
	close(outfile_fd);
	command = ft_split(argv[3], ' ');
	command_execution(command, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	fd;
	int	i;
	pid_t	pid;

	if (argc >= 5)
	{
		i = 1;
		while (argc < 3)
		{
			child_process(argv[i]);
			parent_process();
			i++;
		}
	}
	return (0);
}
