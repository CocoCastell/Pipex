# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>

int	main(int argc, char *argv[], char *envp[])
{
	int	pid;
	int	fd[2];
	int infile;
	int output;

	pipe(fd);
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		return (printf("Erreur\n"), 1);
	output = open(argv[2], O_WRONLY | O_CREAT, 0644);
	if (output == -1)
		return (printf("Erreur2\n"), 1);
	if (dup2(infile, STDIN_FILENO) == -1)
		return (printf("Erreur3\n"), 1);
	pid = fork();
	if (pid == -1)
		return (printf("Erreur4\n"), 1);
	if (pid == 0) { 
		int pid2;
		int fd2[2];
		
		if (pipe(fd2) == -1)
			return (printf("Erreur5\n"), 1);
		pid2 = fork();
		if (pid2 == -1)
			return (printf("Erreur6\n"), 1);
		if (pid2 == 0) {
			char *arg[2];
			close(fd[1]);
			close(fd[0]);
			close(fd2[0]);
			if (dup2(fd2[1], STDOUT_FILENO) == -1)
				return (printf("Erreur7\n"), 1);
			close(fd2[1]);
			arg[0] = "cat";
			arg[1] = NULL;
			execve("/bin/cat", arg, envp);
		}
		else {
			waitpid(pid2, NULL, 0);
			char *arg[3];
			close(fd[0]);
			close(fd2[1]);
			dup2(fd2[0], STDIN_FILENO);
			close(fd2[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			arg[0] = "grep";
			arg[1] = "if only";
			arg[2] = NULL;
			execve("/bin/grep", arg, envp);
			}
	}
	else {
		close(fd[1]);
		if (waitpid(pid, NULL, 0) == -1)
			return (printf("Erreur10\n"), 1);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(output, STDOUT_FILENO);
		close(output);
		char *arg[3];
		arg[0] = "cat";
		arg[1] = NULL;
		execve("/bin/cat", arg, envp);
		return (0);
	}
}
