#ifndef brick_core
#define brick_core

#include <brick_init.h>

typedef struct row_container
{
	int size;
	char *data;
}row_container;

struct brick_win_size
{
	int row;
	int col;
	int current_row;
	int current_column;
	int data_row;
	row_container *container;
};

void brick_core_init(Brick brick);
int brick_read_key(void);
void brick_core_inloop(void);
void brick_clear_screen(void);

#endif