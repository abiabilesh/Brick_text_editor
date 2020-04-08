#include <stdio.h>
#include <unistd.h>
#include <brick_exit.h>

void die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	printf("\r");
	exit(1);
}