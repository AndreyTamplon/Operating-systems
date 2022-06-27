#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include "ctype.h"

int main()
{
	char* socket_path = "./socket";
	struct sockaddr_un addr;
	char buf[BUFSIZ];
	int listener_fd, accepted_client_socket_descriptor;
	ssize_t number_of_characters_read;
	if ((listener_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket error");
		exit(EXIT_FAILURE);
	}
	unlink(socket_path);
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,
			socket_path,
			sizeof(addr.sun_path) - 1);
	if (bind(listener_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("Bind error");
		exit(EXIT_FAILURE);
	}
	if (listen(listener_fd, 5) == -1)
	{
		perror("Listen error");
		exit(EXIT_FAILURE);
	}
	int poll_result;
	const int fds_size = 15;
	struct pollfd fds[fds_size];
	fds[0].fd = listener_fd;
	fds[0].events = POLLIN;
	nfds_t nfds = 1;
	int timeout = 300000;
	while (1)
	{
		poll_result = poll(fds, nfds, timeout);
		if (poll_result < 0)
		{
			perror("Poll error");
			exit(EXIT_FAILURE);
		}
		if (poll_result == 0)
		{
			fprintf(stderr, "Poll timeout\n");
			break;
		}
		int current_number_of_descriptors = nfds;
		for (int i = 0; i < current_number_of_descriptors; ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if (i == 0)
				{
					if((nfds < fds_size))
					{
						if ((accepted_client_socket_descriptor = accept(listener_fd, NULL, NULL)) != -1)
						{
							fds[nfds].fd = accepted_client_socket_descriptor;
							fds[nfds].events = POLLIN;
							nfds++;
							printf("Client connected\n");
						}
						else
						{
							perror("Accept error");
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						fds[0].events = 0;
					}
				}
				else
				{
					number_of_characters_read = read(fds[i].fd, buf, sizeof(buf));
					if (number_of_characters_read == -1)
					{
						perror("Read error");
						exit(EXIT_FAILURE);
					}

					if (number_of_characters_read == 0)
					{
						printf("Connection closed\n");
						close(fds[i].fd);
						for(int j = i; j < nfds; ++j)
						{
							fds[j] = fds[j + 1];
						}
						if(nfds == fds_size - 1)
						{
							fds[0].events = POLLIN;
						}
						nfds--;
					}
					else
					{
						for (int pos = 0; pos < number_of_characters_read; pos++)
						{
							buf[pos] = (char)toupper(buf[pos]);
						}
						printf("%.*s", number_of_characters_read, buf);
					}
				}
			}
		}

	}
	exit(EXIT_SUCCESS);
}

