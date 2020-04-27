#ifndef brick_init
#define brick_init

#include <termios.h>
typedef struct Brick
{
	struct termios org_term;
	int brick_row;
	int brick_column;
	char *file;
}Brick;

#endif


