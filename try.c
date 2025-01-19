#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int	main(int argc, char *argv[]) 
{
	execve("msg.c", argv, NULL);
	return (0);
}
