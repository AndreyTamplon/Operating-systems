#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "provide text source\n");
		exit(EXIT_FAILURE);
	}
	FILE* text_file = fopen(argv[1], "r");
	if (text_file == NULL)
	{
		perror("fopen error");
		exit(EXIT_FAILURE);
	}
	size_t number_of_characters_read;
	FILE* upper_maker = popen("./upperMaker.out", "w");
	if (upper_maker == NULL)
	{
		perror("popen error");
		exit(EXIT_FAILURE);
	}
	char buffer[BUFSIZ];
	while ((number_of_characters_read = fread(buffer, sizeof(char), BUFSIZ, text_file)) > 0)
	{
		if (fwrite(buffer, sizeof(char), number_of_characters_read, upper_maker) < number_of_characters_read)
		{
			perror("write error");
			fclose(text_file);
			pclose(upper_maker);
			exit(EXIT_FAILURE);
		}
	}
	pclose(upper_maker);
	fclose(text_file);
	exit(EXIT_SUCCESS);
}

//интерактивность