#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

int number_of_squeaks = 0;

void PrintError()
{
	char* errno_information = strerror(errno);
	char* error_message = "An error occurred while trying to write information: ";
	write(STDERR_FILENO, error_message, strlen(error_message));
	write(STDERR_FILENO, errno_information, strlen(errno_information));
}

void PrintInformation()
{
	char buffer[40];
	snprintf(buffer, 40, "\nSqueaker squeaked %d times\n", number_of_squeaks);
	if (write(STDOUT_FILENO, buffer, strlen(buffer)) == -1)
	{
		PrintError();
		exit(EXIT_FAILURE);
	}
}

void SignalCatcher(int sig)
{
	if (sig == SIGQUIT)
	{
		PrintInformation();
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGINT)
	{
		if (write(STDOUT_FILENO, "\a", 1) == -1)
		{
			PrintError();
			exit(EXIT_FAILURE);
		}
		number_of_squeaks++;
	}
}

int main()
{
	struct sigaction act;
	act.sa_handler = SignalCatcher;
	sigaddset(&act.sa_mask, SIGQUIT);
	sigaddset(&act.sa_mask, SIGINT);
	if (sigaction(SIGINT, &act, NULL) == -1)
	{
		perror("Failed to change signal SIGINT handler function");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGQUIT, &act, NULL) == -1)
	{
		perror("Failed to change signal SIGQUIT handler function");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		pause();
	}
	exit(EXIT_SUCCESS);
}