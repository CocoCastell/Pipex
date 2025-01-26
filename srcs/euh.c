  1 /* ************************************************************************** */
  2 /*                                                                            */
  3 /*                                                        :::      ::::::::   */
/*   euh.c                                              :+:      :+:    :+:   */
  5 /*                                                    +:+ +:+         +:+     */
  6 /*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
  7 /*                                                +#+#+#+#+#+   +#+           */
  8 /*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:47:08 by cochatel         ###   ########.fr       */
 10 /*                                                                            */
 11 /* ************************************************************************** */
 12 
 13 #include "../includes/pipex.h"
 14 
 15 void    command_execution(char **command, char **envp)
 16 {   
 17     char    *path;
 18     
 19     path = get_path(command, envp);
 20     if (path == NULL)
 21         error_free(command, NULL, NULL, "Command not found");
 22     if (execve((const char *)path, command, envp) == -1)
 23         error_free(command, path, NULL, "Execution error");
 24 }
 25 
 26 void    child_process(char *argv[], char **envp, int pipe_fd[2])
 27 {
 28     char    **command;
 29     int     infile_fd;
 30 
 31     close(pipe_fd[0]);
 32     infile_fd = open(argv[1], O_RDONLY);
 33     if (infile_fd == -1)
 34         fd_error("Error opening first file", pipe_fd[1], -1);
 35     if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
 36         fd_error("Dup2 error", infile_fd, pipe_fd[1]);
 37     close(pipe_fd[1]);
 38     if (dup2(infile_fd, STDIN_FILENO) == -1)
 39         fd_error("Dup2 error", infile_fd, -1);
 40     close(infile_fd);
 41     command = ft_split(argv[2], ' ');
 42     command_execution(command, envp);
 43 }
 45 void    parent_process(char *argv[], char **envp, int pipe_fd[2])
 46 {
 47     char    **command;
 48     int     outfile_fd;
 49 
 50     outfile_fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
 51     if (outfile_fd == -1)
 52         fd_error("Error opening second file", pipe_fd[0], -1);
 53     if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
 54         fd_error("Dup2 error", outfile_fd, pipe_fd[0]);
 55     close(pipe_fd[0]);
 56     if (dup2(outfile_fd, STDOUT_FILENO) == -1)
 57         fd_error("Dup2 errror", outfile_fd, -1);
 58     close(outfile_fd);
 59     command = ft_split(argv[3], ' ');
 60     command_execution(command, envp);
 61 }
 62 
 63 int main(int argc, char *argv[], char *envp[])
 64 {
 65     int     fd[2];
 66     int     status;
 67     pid_t   pid;
 68 
 69     if (argc != 5)
 70         return (ft_printf("Wrong number of arguments\n"), 1);
 71     if (pipe(fd) == -1)
 72         return (ft_printf("Pipe error\n"), 1);
 73     pid = fork();
 74     if (pid == -1)
 75         fd_error("Forking error", fd[0], fd[1]);
 76     if (pid == 0)
 77         child_process(argv, envp, fd);
 78     else
 79     {
 80         close(fd[1]);
 81         if (waitpid(pid, &status, WNOHANG) == -1)
 82             fd_error("Child processus error", fd[0], -1);
 83         if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
 84                 return (close(fd[0]), 1);
 85         parent_process(argv, envp, fd);
 86     }
 87     return (0);
 88 }

