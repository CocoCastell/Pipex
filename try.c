# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>

int	main()
{
	int	pid;
	int	fd[2];

	pipe(fd);
	pid = fork();
	if (pid == 0) {
	int pid2;
	int fd2[2];
	
	pipe(fd2);
	pid2 = fork();
	if (pid2 != 0) {
		int nb2;
		close(fd[1]);
		close(fd2[0]);
		read(fd[0], &nb2, sizeof(nb2));
		close(fd[0]);
		nb2 += 6;
		write(fd2[1], &nb2, sizeof(nb2));
		close(fd2[1]);
	}
	else {
		int	nb3;
		
		close(fd[0]);
		close(fd2[1]);
		read(fd2[0], &nb3, sizeof(nb3));
		close(fd2[0]);
		nb3 += 5;
		write(fd[1], &nb3, sizeof(nb3));
		close(fd[1]);
	}
	} else {
		int nb = 4;
		int new_nb = 0;

		write(fd[1], &nb, sizeof(nb));
		close(fd[1]);
		waitpid(pid, NULL, 0);
		read(fd[0], &new_nb, sizeof(new_nb));
		close(fd[0]);
		write(1, "hola\n", 5);
		printf("nb is: %d\n", new_nb);
	}
}
