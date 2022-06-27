#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
	int file_descriptors[2];
	if (pipe(file_descriptors) == -1)
	{
		perror("failed to set a channel");
		exit(EXIT_FAILURE);
	}
	pid_t subprocess_1, subprocess_2;
	int buffer_size = BUFSIZ;
	char buffer[buffer_size];
	if ((subprocess_1 = fork()) == 0)
	{
		close(file_descriptors[0]);
		ssize_t number_of_characters_read;
		while ((number_of_characters_read = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
		{
			if (write(file_descriptors[1], buffer, number_of_characters_read) == -1)
			{
				perror("Error occurred while writing message to pipe");
				exit(EXIT_FAILURE);
			}
		}
		if (number_of_characters_read == -1)
		{
			perror("Error occurred while reading original message ");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else if (subprocess_1 == -1)
	{
		perror("Failed to create a subprocess");
		exit(EXIT_FAILURE);
	}
	if ((subprocess_2 = fork()) == 0)
	{
		close(file_descriptors[1]);
		ssize_t number_of_characters_read;
		while ((number_of_characters_read = read(file_descriptors[0], buffer, buffer_size)) > 0)
		{
			for (int i = 0; i < number_of_characters_read; ++i)
			{
				printf("%c", (char)toupper(buffer[i]));
			}
		}
		if (number_of_characters_read == -1)
		{
			perror("Error occurred while reading message from pipe");
			exit(EXIT_FAILURE);
		}
		printf("\n");
		exit(EXIT_SUCCESS);
	}
	else if (subprocess_2 == -1)
	{
		perror("Failed to create a subprocess");
		exit(EXIT_FAILURE);
	}
	close(file_descriptors[0]);
	close(file_descriptors[1]);
	while ((wait(NULL)) > 0);
	exit(EXIT_SUCCESS);
}