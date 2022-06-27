#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
	ssize_t number_of_characters_read;
	while ((number_of_characters_read = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
	{
		for (int i = 0; i < number_of_characters_read; ++i)
		{
			printf("%c", (char)toupper(buffer[i]));
		}
	}
	if (number_of_characters_read == -1)
	{
		perror("read error");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}