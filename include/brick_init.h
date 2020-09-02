#ifndef brick_init
#define brick_init

#include <termios.h>
#include <time.h>

typedef struct Brick
{
	struct termios org_term;
	int brick_row;
	int brick_column;
	char message_bar[80];
	time_t message_time;
}Brick;

void brick_main(Brick *brick);
void brick_mbar_init(Brick *brick);
void brick_set_mbar(const char *fmt, ...);

#endif


