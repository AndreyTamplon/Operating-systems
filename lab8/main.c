#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Too few arguments, program requires file name");
		exit(EXIT_FAILURE);
	}
	int fd;
	if ((fd = open(argv[1], O_RDWR)) == -1)
	{
		perror("Couldn't open file");
		exit(EXIT_FAILURE);
	}
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	if (fcntl(fd, F_SETLK, &lock) == -1)
	{
		if ((errno == EACCES) || (errno == EAGAIN))
		{
			printf("%s busy -- try later\n", argv[1]);
		}
		else
		{
			perror("Failed to set up lock");
		}
		close(fd);
		exit(EXIT_FAILURE);
	}
	char buffer[strlen(argv[1]) + 10];
	snprintf(buffer, strlen(argv[1]) + 10, "nano %s", argv[1]);
	if(system(buffer) == -1)
	{
		perror("Couldn't open file in nano");
		close(fd);
		exit(EXIT_FAILURE);
	}
	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Failed to unlock file");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
