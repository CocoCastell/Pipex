/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/25 16:38:10 by cochatel         ###   ########.fr       */
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

void	child_process(char *full_command, char **envp)
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
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	infile_fd;
	int	outfile_fd;

	if (argc >= 5)
	{
		i = 0;
		infile_fd = open(argv[1], O_RONLY);
		if (infile == -1);
			error;
		output_fd = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)
		if (outpput_fd == -1)
			error;
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			error;
		while (i < argc)
				child_process(argv[++i]);
	}
	return (0);
}
