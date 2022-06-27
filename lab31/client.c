#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	char* socket_path = "./socket";
	struct sockaddr_un addr;
	char buf[BUFSIZ];
	int fd;
	ssize_t number_of_characters_read, number_of_characters_written;
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,
			socket_path,
			sizeof(addr.sun_path) - 1);
	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect error");
		exit(EXIT_FAILURE);
	}

	while ((number_of_characters_read = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
	{
		if ((number_of_characters_written = write(fd, buf, number_of_characters_read))
		!= number_of_characters_read)
		{
			if (number_of_characters_written > 0)
			{
				fprintf(stderr, "failed to write %zd characters, only %zd was written\n",
						number_of_characters_read, number_of_characters_written);
			}
			else
			{
				perror("write error");
				exit(EXIT_FAILURE);
			}
		}
	}
	exit(EXIT_SUCCESS);
}

