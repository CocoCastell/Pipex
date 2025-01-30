/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/30 15:11:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// data[0] -> argc
// data[1] -> argument index
// data[2] -> here_doc option (1) or not (0)

void	process(char *argv[], char *envp[], int data[3])
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		fd_error("Pipe error", pipe_fd[0], -1, -1);
	pid = fork();
	if (pid == -1)
		fd_error("Fork error", pipe_fd[0], pipe_fd[1], pipe_fd[0]);
	if (pid == 0)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[1], pipe_fd[0], -1);
		close_fd(pipe_fd[1], pipe_fd[0], -1);
		command_execution(argv, envp, data);
	}
	else
	{
		while (waitpid(pid, NULL, 0) > 0)
			data[2] += 1 - 1;
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			fd_error("Error dup2", pipe_fd[1], pipe_fd[0], -1);
		close_fd(pipe_fd[1], pipe_fd[0], -1);
		data[1]++;
		pipex_recursion(argv, envp, data);
	}
}

void	pipex_recursion(char *argv[], char *envp[], int data[3])
{
	if (data[1] == data[0] - 2)
		outfile_command(argv, envp, data);
	else
		process(argv, envp, data);
}

void	file_init(int argc, char *argv[], char *envp[])
{
	int	infile_fd;
	int	data[3];

	data[0] = argc;
	data[1] = 2;
	data[2] = 0;
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		infile_fd = open("/dev/null", O_RDONLY);
		if (infile_fd == -1)
			fd_error("Error opening file", -1, -1, -1);
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		fd_error("Error dup2", infile_fd, -1, -1);
	close(infile_fd);
	pipex_recursion(argv, envp, data);
}

void	here_doc_init(int argc, char *argv[], char *envp[])
{
	int		data[3];
	int		pipe_fd[2];
	char	*string;
	size_t	string_length;

	data[0] = argc;
	data[1] = 3;
	data[2] = 1;
	if (pipe(pipe_fd) == -1)
		fd_error("Pipe error", -1, -1, -1);
	while (1)
	{
		string = get_next_line(0);
		string_length = ft_strlen(string);
		if (ft_strncmp(argv[2], string, --string_length) == 0)
			break ;
		write(pipe_fd[1], string, sizeof(char) * ++string_length);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		fd_error("Error dup2", pipe_fd[0], -1, -1);
	close(pipe_fd[0]);
	pipex_recursion(argv, envp, data);
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
